--- Source/WebCore/platform/graphics/cpu/arm/filters/FELightingNEON.h.orig	2018-04-18 06:26:41 UTC
+++ Source/WebCore/platform/graphics/cpu/arm/filters/FELightingNEON.h
@@ -144,9 +144,9 @@ inline void FELighting::platformApplyNeo
             neonData.flags |= FLAG_CONE_EXPONENT_IS_1;
     } else {
         ASSERT(m_lightSource->type() == LS_DISTANT);
-        floatArguments.lightX = paintingData.lightVector.x();
-        floatArguments.lightY = paintingData.lightVector.y();
-        floatArguments.lightZ = paintingData.lightVector.z();
+        floatArguments.lightX = paintingData.initialLightingData.lightVector.x();
+        floatArguments.lightY = paintingData.initialLightingData.lightVector.y();
+        floatArguments.lightZ = paintingData.initialLightingData.lightVector.z();
         floatArguments.padding2 = 1;
     }
 
