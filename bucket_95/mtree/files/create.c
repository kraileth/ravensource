#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <fts.h>
#include <strings.h>
#include <grp.h>
#ifdef MD5
#include <md5.h>
#endif
#ifdef USE_SHA1
#include "sha.h"
#include "sha1hl.h"
#endif
#ifdef RMD160
#include <ripemd.h>
#endif
#include <pwd.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <vis.h>
#include "mtree.h"
#include "extern.h"

#ifndef __printflike
#define	__printflike(fmtarg, firstvararg) \
            __attribute__((__nonnull__(fmtarg), \
			  __format__ (__printf__, fmtarg, firstvararg)))
#endif
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	256
#endif

#define	INDENTNAMELEN	15
#define	MAXLINELEN	80

static gid_t gid;
static uid_t uid;
static mode_t mode;
static u_long flags = 0xffffffff;

#ifdef __sunlinux__
static int	dsort(const FTSENT **, const FTSENT **);
#else
static int	dsort(const FTSENT * const *, const FTSENT * const *);
#endif
static void	output(int, int *, const char *, ...) __printflike(3, 4);
static int	statd(FTS *, FTSENT *, uid_t *, gid_t *, mode_t *,
			   u_long *);
static void	statf(int, FTSENT *);

void
cwalk(void)
{
	FTS *t;
	FTSENT *p;
	time_t clk;
	char *argv[2], host[MAXHOSTNAMELEN], dot[] = ".";
	int indent = 0;

	time(&clk);
	gethostname(host, sizeof(host));
	printf(
	    "#\tmachine: %s\n#\t   tree: %s\n#\t   date: %s",
	    host, fullpath, ctime(&clk));
	argv[0] = dot;
	argv[1] = NULL;
	if ((t = fts_open(argv, ftsoptions, dsort)) == NULL)
		err(1, "line %d: fts_open", lineno);
	while ((p = fts_read(t))) {
		if (iflag)
			indent = p->fts_level * 4;
		if (check_excludes(p->fts_name, p->fts_path)) {
			fts_set(t, p, FTS_SKIP);
			continue;
		}
		switch(p->fts_info) {
		case FTS_D:
			if (!dflag)
				printf("\n");
			if (!nflag)
				printf("# %s\n", p->fts_path);
			statd(t, p, &uid, &gid, &mode, &flags);
			statf(indent, p);
			break;
		case FTS_DP:
			if (!nflag && (p->fts_level > 0))
				printf("%*s# %s\n", indent, "", p->fts_path);
			printf("%*s..\n", indent, "");
			if (!dflag)
				printf("\n");
			break;
		case FTS_DNR:
		case FTS_ERR:
		case FTS_NS:
			warnx("%s: %s", p->fts_path, strerror(p->fts_errno));
			break;
		default:
			if (!dflag)
				statf(indent, p);
			break;

		}
	}
	fts_close(t);
	if (sflag && keys & F_CKSUM)
		warnx("%s checksum: %u", fullpath, crc_total);
}

static void
statf(int indent, FTSENT *p)
{
	struct group *gr;
	struct passwd *pw;
	u_long len, val;
	int fd, offset;
	char *fflags;
	char *escaped_name;

	escaped_name = calloc(1, p->fts_namelen * 4  +  1);
	if (escaped_name == NULL)
		errx(1, "statf(): calloc() failed");
	strvis(escaped_name, p->fts_name, VIS_WHITE | VIS_OCTAL);

	if (iflag || S_ISDIR(p->fts_statp->st_mode))
		offset = printf("%*s%s", indent, "", escaped_name);
	else
		offset = printf("%*s    %s", indent, "", escaped_name);
	
	free(escaped_name);

	if (offset > (INDENTNAMELEN + indent))
		offset = MAXLINELEN;
	else
		offset += printf("%*s", (INDENTNAMELEN + indent) - offset, "");

	if (!S_ISREG(p->fts_statp->st_mode) && !dflag)
		output(indent, &offset, "type=%s", inotype(p->fts_statp->st_mode));
	if (p->fts_statp->st_uid != uid) {
		if (keys & F_UNAME) {
			if ((pw = getpwuid(p->fts_statp->st_uid)) != NULL) {
				output(indent, &offset, "uname=%s", pw->pw_name);
			} else {
				errx(1,
				"line %d: could not get uname for uid=%u",
				lineno, p->fts_statp->st_uid);
			}
		}
		if (keys & F_UID)
			output(indent, &offset, "uid=%u", p->fts_statp->st_uid);
	}
	if (p->fts_statp->st_gid != gid) {
		if (keys & F_GNAME) {
			if ((gr = getgrgid(p->fts_statp->st_gid)) != NULL) {
				output(indent, &offset, "gname=%s", gr->gr_name);
			} else {
				errx(1,
				"line %d: could not get gname for gid=%u",
				lineno, p->fts_statp->st_gid);
			}
		}
		if (keys & F_GID)
			output(indent, &offset, "gid=%u", p->fts_statp->st_gid);
	}
	if (keys & F_MODE && (p->fts_statp->st_mode & MBITS) != mode)
		output(indent, &offset, "mode=%#o", p->fts_statp->st_mode & MBITS);
	if (keys & F_NLINK && p->fts_statp->st_nlink != 1)
		output(indent, &offset, "nlink=%u", (unsigned int)p->fts_statp->st_nlink);
	if (keys & F_SIZE)
		output(indent, &offset, "size=%jd",
		    (uintmax_t)p->fts_statp->st_size);
	if (keys & F_TIME)
		output(indent, &offset, "time=%ld.%ld",
#ifdef __sunlinux__
		    p->fts_statp->st_mtim.tv_sec,
		    p->fts_statp->st_mtim.tv_nsec);
#else
		    p->fts_statp->st_mtimespec.tv_sec,
		    p->fts_statp->st_mtimespec.tv_nsec);
#endif
	if (keys & F_CKSUM && S_ISREG(p->fts_statp->st_mode)) {
		if ((fd = open(p->fts_accpath, O_RDONLY, 0)) < 0 ||
		    crc(fd, &val, &len))
			err(1, "line %d: %s", lineno, p->fts_accpath);
		close(fd);
		output(indent, &offset, "cksum=%lu", val);
	}
#ifdef MD5
	if (keys & F_MD5 && S_ISREG(p->fts_statp->st_mode)) {
		char *digest, buf[33];

		digest = MD5File(p->fts_accpath, buf);
		if (!digest) {
			err(1, "line %d: %s", lineno, p->fts_accpath);
		} else {
			output(indent, &offset, "md5digest=%s", digest);
		}
	}
#endif /* MD5 */
#ifdef USE_SHA1
	if (keys & F_SHA1 && S_ISREG(p->fts_statp->st_mode)) {
		char *digest, buf[41];

		digest = SHA1_File(p->fts_accpath, buf);
		if (!digest) {
			err(1, "line %d: %s", lineno, p->fts_accpath);
		} else {
			output(indent, &offset, "sha1digest=%s", digest);
		}
	}
#endif /* SHA1 */
#ifdef RMD160
	if (keys & F_RMD160 && S_ISREG(p->fts_statp->st_mode)) {
		char *digest, buf[41];

		digest = RIPEMD160_File(p->fts_accpath, buf);
		if (!digest) {
			err(1, "line %d: %s", lineno, p->fts_accpath);
		} else {
			output(indent, &offset, "ripemd160digest=%s", digest);
		}
	}
#endif /* RMD160 */
	if (keys & F_SLINK &&
	    (p->fts_info == FTS_SL || p->fts_info == FTS_SLNONE))
		output(indent, &offset, "link=%s", rlink(p->fts_accpath));
#ifndef __sunlinux__
	if (keys & F_FLAGS && p->fts_statp->st_flags != flags) {
		fflags = flags_to_string(p->fts_statp->st_flags);
		output(indent, &offset, "flags=%s", fflags);
		free(fflags);
	}
#endif /* ! __sunlinux__ */
	putchar('\n');
}

#define	TREE_MAXGID	5000
#define	TREE_MAXUID	5000
#define	TREE_MAXMODE	MBITS + 1
#define	TREE_MAXFLAGS	256
#define	MAXS 16

static int
statd(FTS *t, FTSENT *parent, uid_t *puid, gid_t *pgid, mode_t *pmode,
      u_long *pflags)
{
	FTSENT *p;
	gid_t sgid;
	uid_t suid;
	mode_t smode;
	u_long sflags;
	struct group *gr;
	struct passwd *pw;
	gid_t savegid = *pgid;
	uid_t saveuid = *puid;
	mode_t savemode = *pmode;
	u_long saveflags = *pflags;
	u_short maxgid, maxuid, maxmode, maxflags;
	u_short g[TREE_MAXGID], u[TREE_MAXUID], m[TREE_MAXMODE], f[TREE_MAXFLAGS];
	char *fflags;
	static int first = 1;

	if ((p = fts_children(t, 0)) == NULL) {
		if (errno)
			err(1, "line %d: %s", lineno, RP(parent));
		return (1);
	}

	bzero(g, sizeof(g));
	bzero(u, sizeof(u));
	bzero(m, sizeof(m));
	bzero(f, sizeof(f));

	maxuid = maxgid = maxmode = maxflags = 0;
	for (; p; p = p->fts_link) {
		if (!dflag || (dflag && S_ISDIR(p->fts_statp->st_mode))) {
			smode = p->fts_statp->st_mode & MBITS;
			if (smode < TREE_MAXMODE && ++m[smode] > maxmode) {
				savemode = smode;
				maxmode = m[smode];
			}
			sgid = p->fts_statp->st_gid;
			if (sgid < TREE_MAXGID && ++g[sgid] > maxgid) {
				savegid = sgid;
				maxgid = g[sgid];
			}
			suid = p->fts_statp->st_uid;
			if (suid < TREE_MAXUID && ++u[suid] > maxuid) {
				saveuid = suid;
				maxuid = u[suid];
			}

#ifndef __sunlinux__
			/*
			 * XXX
			 * note that the below will break when file flags
			 * are extended beyond the first 4 bytes of each
			 * half word of the flags
			 */
#define FLAGS2IDX(f) ((f & 0xf) | ((f >> 12) & 0xf0))
			sflags = p->fts_statp->st_flags;
			if (FLAGS2IDX(sflags) < TREE_MAXFLAGS &&
			    ++f[FLAGS2IDX(sflags)] > maxflags) {
				saveflags = sflags;
				maxflags = f[FLAGS2IDX(sflags)];
			}
#endif /* ! __sunlinux__ */
		}
	}
	/*
	 * If the /set record is the same as the last one we do not need to output
	 * a new one.  So first we check to see if anything changed.  Note that we
	 * always output a /set record for the first directory.
	 */
	if ((((keys & F_UNAME) | (keys & F_UID)) && (*puid != saveuid)) ||
	    (((keys & F_GNAME) | (keys & F_GID)) && (*pgid != savegid)) ||
	    ((keys & F_MODE) && (*pmode != savemode)) || 
#ifndef __sunlinux__
	    ((keys & F_FLAGS) && (*pflags != saveflags)) ||
#endif /* ! __sunlinux__ */
	    (first)) {
		first = 0;
		if (dflag)
			printf("/set type=dir");
		else
			printf("/set type=file");
		if (keys & F_UNAME) {
			if ((pw = getpwuid(saveuid)) != NULL)
				printf(" uname=%s", pw->pw_name);
			else
				errx(1,
				"line %d: could not get uname for uid=%u",
				lineno, saveuid);
		}
		if (keys & F_UID)
			printf(" uid=%lu", (u_long)saveuid);
		if (keys & F_GNAME) {
			if ((gr = getgrgid(savegid)) != NULL)
				printf(" gname=%s", gr->gr_name);
			else
				errx(1,
				"line %d: could not get gname for gid=%u",
				lineno, savegid);
		}
		if (keys & F_GID)
			printf(" gid=%lu", (u_long)savegid);
		if (keys & F_MODE)
			printf(" mode=%#o", savemode);
		if (keys & F_NLINK)
			printf(" nlink=1");
#ifndef __sunlinux__
		if (keys & F_FLAGS) {
			fflags = flags_to_string(saveflags);
			printf(" flags=%s", fflags);
			free(fflags);
		}
#endif /* ! __sunlinux__ */
		printf("\n");
		*puid = saveuid;
		*pgid = savegid;
		*pmode = savemode;
		*pflags = saveflags;
	}
	return (0);
}

static int
#ifdef __sunlinux__
dsort(const FTSENT **a, const FTSENT **b)
#else
dsort(const FTSENT * const *a, const FTSENT * const *b)
#endif
{

	if (S_ISDIR((*a)->fts_statp->st_mode)) {
		if (!S_ISDIR((*b)->fts_statp->st_mode))
			return (1);
	} else if (S_ISDIR((*b)->fts_statp->st_mode))
		return (-1);
	return (strcmp((*a)->fts_name, (*b)->fts_name));
}

void
output(int indent, int *offset, const char *fmt, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	if (*offset + strlen(buf) > MAXLINELEN - 3) {
		printf(" \\\n%*s", INDENTNAMELEN + indent, "");
		*offset = INDENTNAMELEN + indent;
	}
	*offset += printf(" %s", buf) + 1;
}
