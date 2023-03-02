#pragma once

#include <aws/core/http/HttpRequest.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/Object.h>
#include <aws/s3/model/PutObjectRequest.h>

using namespace Aws::S3;
using namespace Aws::S3::Model;

namespace ykaws {

class Minio {
private:
    Aws::SDKOptions options;
    S3Client *client_p;
    bool status;

public:
    Minio();
    ~Minio();

    void Init(std::string host, std::string port, std::string ak, std::string sk);

    bool check_bucket_exist(std::string bucket_name);
    bool create_bucket(std::string bucket_name);
    bool upload_str_stream(std::string bucket_name, std::string object_name, std::string object_type, std::vector<char> buffer);
};

}