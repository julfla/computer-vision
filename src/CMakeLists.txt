ADD_EXECUTABLE( k-means main.cxx ${SRC})
TARGET_LINK_LIBRARIES( k-means ${OpenCV_LIBS} ${Boost_LIBRARIES})
