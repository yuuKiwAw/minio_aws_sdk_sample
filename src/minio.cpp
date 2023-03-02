#include "minio.hpp"

namespace ykaws {

Minio::Minio() {}

Minio::~Minio() {
    delete client_p; client_p = nullptr;
    Aws::ShutdownAPI(options);
}

void Minio::Init(std::string host, std::string port, std::string ak, std::string sk) {
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration cfg;
    cfg.endpointOverride = host + ":" + port;
    cfg.scheme = Aws::Http::Scheme::HTTP;
    cfg.verifySSL = false;

    Aws::Auth::AWSCredentials cred(ak, sk);
    client_p = new S3Client(cred, cfg, Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Always, false);

    auto response = client_p->ListBuckets();
    if (response.IsSuccess()) {
        std::cout << "[MINIO] MINIO init SUCCESS! Auth Pass!" << std::endl;
        status = true;
    } else {
        std::cout << "[MINIO] MINIO init FAILED! No Auth!" << std::endl;
        status = false;
    }
}

bool Minio::check_bucket_exist(std::string bucket_name) {
    Aws::S3::Model::ListObjectsRequest request;
    request.WithBucket(bucket_name);
    auto response = client_p->ListObjects(request);
    return response.IsSuccess() ? true : false;
}

bool Minio::create_bucket(std::string bucket_name) {
    Aws::S3::Model::CreateBucketRequest request;
    request.SetBucket(bucket_name);
    auto reponse = client_p->CreateBucket(request);
    return reponse.IsSuccess() ? true : false;
}

/**
 * @param object_name
 * The tag-set for the object. The tag-set must be encoded as URL Query
 * parameters. (For example, "Key1=Value1")</p>
*/
bool Minio::upload_str_stream(std::string bucket_name, std::string object_name, std::string object_type, std::vector<char> buffer) {
    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucket_name);
    request.SetKey(object_name);
    request.SetTagging(object_type);

    auto input_data = Aws::MakeShared<Aws::StringStream>("");
    input_data->write(buffer.data(), buffer.size());
    request.SetBody(input_data);

    auto putObjectOutcome = client_p->PutObject(request);
    return putObjectOutcome.IsSuccess() ? true : false;
}

}