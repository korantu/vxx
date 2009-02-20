-include main.dep

#assemble flags:

INCLUDES := -I$(ATB_INCLUDE) -I$(GLFW_INCLUDE) -I$(GTEST_INCLUDE)
CXXFLAGS := $(CXXFLAGS) -Wall -g -O3 $(INCLUDES)
LINKFLAGS := $(ATB_STATIC) $(GLFW_STATIC) $(GTEST_STATIC) $(LINKFLAGS) 


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

vxVector.t: vxVector_UT.o vxVector.o
vxRange.t: vxRange_UT.o vxRange.o
vxScene.a: vxAction.o vxScene.o vxDrawSphere.o vxDrawPlane.o vxDrawSurface.o vxVector.o vxProjection.o vxOpenGlTools.o vxRay.o vxRange.o vxLighting.o
vxScene.t: vxScene_UT.o vxScene.a
vxAction.t: vxAction_UT.o vxAction.o
vxProjection.t: vxProjection_UT.o vxProjection.o vxVector.o
vxMouseRay.t: vxMouseRay_UT.o vxScene.a
vxDrawSurface.t: vxDrawSurface_UT.o vxScene.a
vxFastVolume.t: vxFastVolume_UT.o vxFastVolume.o vxLoader.o vxFileGzipIo.o vxValidatable.o vxSmoothBell.o vxColor.o vxColorLookupTable.o  

clean:
	rm *.o 

.PHONY: clean tags deps


