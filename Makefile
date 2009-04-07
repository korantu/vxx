-include main.dep

#assemble flags:

INCLUDES := -I$(ATB_INCLUDE) -I$(GLFW_INCLUDE) -I$(GTEST_INCLUDE) -I$(FTGL_INCLUDE)
CXXFLAGS := $(CXXFLAGS) -Wall -g -O3 $(INCLUDES) `freetype-config --cflags`
LINKFLAGS := $(ATB_STATIC) $(GLFW_STATIC) $(GTEST_STATIC) $(FTGL_STATIC) $(LINKFLAGS)


#Rules
%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@  

%.t: $^
	$(CXX) $^ $(CXXFLAGS) $(LINKFLAGS)  -o $@

%.a: $^
	ar r $@ $^

#build dependencies using g++
deps: 
	$(CXX) $(CXXFLAGS) *.cpp -MM > main.dep 

tags:
	etags $(ETAGSFLAGS) *.cpp *.h $(GLFW_INCLUDE)/GL/glfw.h

git:
	git remote add origin git@github.com:korantu/vxx.git

vxVector.t: vxVector_UT.o vxVector.o
vxRange.t: vxRange_UT.o vxRange.o
vxScene.a: vxAction.o vxScene.o vxDrawSphere.o vxDrawPlane.o vxDrawSurface.o vxValidatable.o vxColor.o vxColorLookupTable.o vxSmoothBell.o vxFileGzipIo.o vxFollower.o vxFastVolume.o vxTextured.o vxDrawSurface.o vxSurface.o vxVector.o vxProjection.o vxMotion.o vxOpenGlTools.o vxRay.o vxRange.o vxLighting.o vxMouseRay.o vxFontFtgl.o vxBinaryBlobs.o vxSurfaceSlicer.o 
vxScene.t: vxScene_UT.o vxScene.a
vxRay.t: vxRay_UT.o vxScene.a
vxAction.t: vxAction_UT.o vxAction.o vxScene.a
vxProjection.t: vxProjection_UT.o vxProjection.o vxVector.o
vxMouseRay.t: vxMouseRay_UT.o vxMouseRay.o vxScene.a
vxTouchAction.t: vxTouchAction_UT.o vxTouchAction.o
vxDrawSurface.t: vxDrawSurface_UT.o vxScene.a
vxDrawSphere.t: vxDrawSphere_UT.o vxScene.a
vxFastVolume.a: vxFastVolume.o vxLoader.o vxFileGzipIo.o vxValidatable.o vxSmoothBell.o vxColor.o vxColorLookupTable.o  
vxFastVolume.t: vxFastVolume_UT.o vxFastVolume.a
vxSurface.t: vxSurface_UT.o vxSurface.o vxScene.a vxFastVolume.a
vxOpenGlTools.t: vxOpenGlTools_UT.o vxScene.a 
vxTextured.t: vxDrawSurface.o vxTextured_UT.o vxTextured.o vxLoader.o vxScene.a
vxTest.t: vxTest_UT.o
vxFollower.t:  vxFollower.o vxFollower_UT.o
vxFontFtgl.t:  vxFontFtgl.o vxFontFtgl_UT.o vxScene.a
vxSurfaceSlicer.t:  vxSurfaceSlicer.o vxSurfaceSlicer_UT.o  vxScene.a 
vxx.t: main.o vxDrawSurface.o vxTextured.o vxLoader.o vxPatientsNavigation.o vxScene.a

clean:
	rm *.o *.a

.PHONY: clean tags deps git


