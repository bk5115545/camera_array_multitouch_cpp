set(TARGET_NAME camera_array_multitouch_cpp)
file(GLOB SOURCE_FILES
    "*.cpp"
    "*.c"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PRIVATE
                          "${OpenCV_INCLUDE_DIRS}"
			                    "${Boost_INCLUDE_DIRS}"
                          "${Boost_FILESYSTEM_LIBRARY}"
                          "${Boost_SYSTEM_LIBRARY}"
                          "${CMAKE_SOURCE_DIR}/include")

target_link_libraries("${TARGET_NAME}" "${OpenCV_LIBS}" "${Boost_LIBRARIES}")
