#include "request_queue.h"

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    return AddFindRequest(raw_query,
        [status](int, DocumentStatus document_status, int) {
            return document_status == status;
        });
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    return AddFindRequest(raw_query,
        [](int, DocumentStatus document_status, int) {
            return document_status == DocumentStatus::ACTUAL;
        });
}

size_t RequestQueue::GetNoResultRequests() const {
    return requests_.size();
}