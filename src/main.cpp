#include <iostream>
#include <aws/core/http/HttpRequest.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/Object.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace Aws::S3;
using namespace Aws::S3::Model;

std::vector<char> mat2buffer() {
    cv::Mat mat = cv::imread("C:/Users/c5446/Downloads/R-C.jpg", 1);
    std::vector<uchar> data_encode;
    cv::imencode(".jpg", mat, data_encode);
    std::vector<char> data_encode_(data_encode.begin(), data_encode.end());

    return data_encode_;
}

bool check_bucket_exist(S3Client &client, std::string bucket_name) {
    Aws::S3::Model::ListObjectsRequest objects_request;
    objects_request.WithBucket(bucket_name);
    auto list_objects_outcome = client.ListObjects(objects_request);
    if (list_objects_outcome.IsSuccess())
    {
        return true;
    } else {
        return false;
    }
}

bool create_bucket(S3Client &client, std::string bucket_name) {
    Aws::S3::Model::CreateBucketRequest create_bucket_request;
    create_bucket_request.SetBucket(bucket_name);

    auto create_bucket_outcome = client.CreateBucket(create_bucket_request);
    if (create_bucket_outcome.IsSuccess()) {
        std::cout << "[MINIO] Create bucket SUCCESS!" << std::endl;
        return true;
    } else {
        std::cout << "[MINIO] Create bucket FAILED!" << std::endl;
        // std::cout << "[MINIO] [ERROR]: " << create_bucket_outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool upload_iostream(S3Client &client, std::string bucket_name, std::string object_name, std::vector<char> buffer) {
    Aws::S3::Model::PutObjectRequest putObjectRequest;
    putObjectRequest.SetBucket(bucket_name);
    putObjectRequest.SetKey(object_name);

    auto input_data = Aws::MakeShared<Aws::StringStream>("");
    input_data->write(buffer.data(), buffer.size());
    putObjectRequest.SetBody(input_data);

    auto putObjectOutcome = client.PutObject(putObjectRequest);
    if (putObjectOutcome.IsSuccess()) {
        std::cout << "success" << std::endl;
        return true;
    } else {
        std::cout << "failed" << std::endl;
        std::cout << putObjectOutcome.GetError() << std::endl;
        return false;
    }
}

int main(int argc, char *argv[]) {
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration cfg;
    cfg.endpointOverride = "127.0.0.1:9000";
    cfg.scheme = Aws::Http::Scheme::HTTP;
    cfg.verifySSL = false;

    Aws::Auth::AWSCredentials cred("V7sdnmS4JTnzYGwG", "7h2U17RuSZ1KSh0R3C2Zf88L7XzTcRUt");
    S3Client client(cred, cfg, Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Always, false);

    auto response = client.ListBuckets();
    if (response.IsSuccess()) {
        std::cout << "[MINIO] MINIO init SUCCESS! Auth Pass!" << std::endl;
        // create_bucket(client, "test");
        upload_iostream(client, "pbr-intjet-detect", "a2.jpg", mat2buffer());
    } else {
        std::cout << "[MINIO] MINIO init FAILED! No Auth!" << std::endl;
    }

    Aws::ShutdownAPI(options);

    return 0;
}