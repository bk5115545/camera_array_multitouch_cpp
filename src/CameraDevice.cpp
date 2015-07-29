#include "CameraDevice.h"
//#include "vld.h"

CameraDevice::CameraDevice(int camera_id)
	: camera_id(camera_id)
	, frame_id(0)
{
//	this->camera_id = camera_id;

//	this->frame_id = 0;
}

CameraDevice::~CameraDevice() {
	capture.release();
}

bool CameraDevice::acquire() {
	if (acquired) return true;

	capture = cv::VideoCapture(camera_id);
	if (capture.isOpened()) {
		acquired = true;
		//capture.set(CV_CAP_PROP_SETTINGS,1); //open driver advanced settings panel
		return true;
	}
	
	return false;
}

void CameraDevice::release() {
	if (!acquired) return;

	acquired = false;
	capture.release();
}

/*
	Returns a Frame pointer to a frame allocated on the heap.
	You must ensure that this pointer is deleted.
*/
std::shared_ptr<Frame> CameraDevice::getFrame() {
	cv::Mat mat;
	capture.read(mat);
	
	return std::make_shared<Frame>(Frame(mat, camera_id)); 
}

/*
	Grabs the next frame but does not decode, useful
	for decoding for special cameras (such as depth or Kinect)
	or multi-camera environments

	decodeFrame needs to be called after this to decode.

	OUTPUT:
		false if unsuccessful
*/
bool CameraDevice::grabFrame() {
	return capture.grab();
}

/*
	Decodes a frame based on a channnel

	INPUT:
		An OpenCV Mat to store the decoded frame
		An int channel number

	OUTPUT:
		Frame* to the decoded frame
*/
std::shared_ptr<Frame> CameraDevice::decodeFrame(int channel) {
	cv::Mat mat;
	capture.retrieve(mat, channel); // ::TODO:: Error Catching

	return std::make_shared<Frame>(Frame(mat, camera_id));
}

int CameraDevice::getID() {
	return camera_id;
}

/*
	Calibrates the camera device based on Chessboard

	::TODO:: Test this code
	
	OUTPUT:
		false if unsuccessful
*/
bool CameraDevice::calibrate_lens() {
	cv::Mat mat;

	std::vector<cv::Point2f> corners; // detected corners
	cv::Size pattern_size(8, 6); // number of interior corners

	// Quickly find a chessboard's corners
	// or try for 100 frames.
	int max_iterations = 1000;
	int cur_iteration = 0;
	
	do {
		capture.read(mat);
		cur_iteration++;
	} while (!cv::findChessboardCorners(mat, pattern_size, corners, 
				cv::CALIB_CB_ADAPTIVE_THRESH	// Uses Adaptive Thresholding to convert to grayscale
				+ cv::CALIB_CB_NORMALIZE_IMAGE	// Uses equalizeHist for normalization
				+ cv::CALIB_CB_FAST_CHECK		// Fast check for chessboard
			) || cur_iteration >= max_iterations
			);

	// if no chessboard was found
	if (cur_iteration == max_iterations)
		return false;

	// Better approximate the corner positions
	cv::cornerSubPix(mat, corners, cv::Size(11, 11), cv::Size(-1, -1),
		cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

	//cv::calibrateCamera();

	return true;
}

/*
	Gets the FPS of a camera

	::TODO:: Test on a video feed

	OUTPUT:
		Camera FPS as a double
*/
inline double CameraDevice::getFPS() {
	return capture.get(CV_CAP_PROP_FPS);
}

/*
	Sets the FPS of a camera

	::TODO:: Test on a video feed

	INPUT:
		FPS as a double

	OUTPUT:
		false if unsuccessful
*/
inline bool CameraDevice::setFPS(double value) {
	return capture.set(CV_CAP_PROP_FPS, value);
}

/*
	OUTPUT:
		set_values[0] false if get height is unsuccessful
		set_values[1] false if get width is unsuccessful
*/
inline std::pair<double, double> CameraDevice::getResolution() {
	return std::pair<double, double>(capture.get(CV_CAP_PROP_FRAME_HEIGHT), capture.get(CV_CAP_PROP_FRAME_WIDTH));
}

/*
	INPUT:
		height as a double
		width as a double

	OUTPUT:
		err_flags[0] false if set height is unsuccessful
		err_flags[1] false if set width is unsuccessful
*/
inline std::pair<bool, bool> CameraDevice::setResolution(double height, double width) {
	return std::pair<bool, bool>(capture.set(CV_CAP_PROP_FRAME_HEIGHT, height), capture.set(CV_CAP_PROP_FRAME_WIDTH, width));
}

/*
	INPUT:
		http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-get

	OUTPUT:
		0 if get was unsuccessful
*/
inline double CameraDevice::getOpenCVProperty(int prop_id) {
	return capture.get(prop_id); //::TODO:: Do error catching
}

/*
	INPUT:
		http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-get

	OUTPUT:
		false if set was unsuccessful
*/
inline bool CameraDevice::setOpenCVProperty(int prop_id, double value) {
	return capture.set(prop_id, value);
}
