#include <iostream>
#include <opencv2/opencv.hpp>
#include "minio.hpp"

int main(int argc, char *argv[]) {
    // read mat to buffer
    cv::Mat mat = cv::imread("C:/Users/c5446/Downloads/R-C.jpg", 1);
    std::vector<uchar> data_encode;
    cv::imencode(".jpg", mat, data_encode);
    std::vector<char> data_encode_(data_encode.begin(), data_encode.end());

    // minio
    ykaws::Minio *minio_sdk = new ykaws::Minio();
    minio_sdk->Init("127.0.0.1", "9000",
                    "V7sdnmS4JTnzYGwG",
                    "7h2U17RuSZ1KSh0R3C2Zf88L7XzTcRUt");
    if (!minio_sdk->check_bucket_exist("test"))
        minio_sdk->create_bucket("test") ? std::cout << "create success" : std::cout << "create failed";
    minio_sdk->upload_str_stream("test", "a2.jpg", "provenance=Nier",data_encode_) ? std::cout << "upload success" : std::cout << "upload failed";

    delete minio_sdk; minio_sdk = nullptr;

    return 0;
}