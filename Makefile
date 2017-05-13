CC=g++
src=skin_dect/skin_dec.cpp skin_dect/bmp.cpp
CV_INC= -I/usr/include/opencv2
CV_LIB= -lopencv_imgproc -lopencv_features2d -lopencv_core -lopencv_highgui
CFLAG= -g -Ofast 
 
.PHONY:clean skin_dect

skin_dect:
	rm -rf SKIN *.*~ *.o  CAM_SKIN 
	$(CC) $(src) $(CFLAG) -c 
	$(CC) *.o skin_dect/bmp.h -o SKIN
open_cam:
	rm -rf SKIN *.*~ *.o  CAM_SKIN 
	$(CC) skin_dect/open_cam.cpp $(CV_INC) $(CV_LIB) -o CAM_SKIN 
clean:
	rm -rf SKIN *.*~ *.o *~  CAM_SKIN 
