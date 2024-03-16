#include "document.h"

Document::Document() = default;

Document::Document(int document_id, double document_relevance, int document_rating)
    : id(document_id)
    , relevance(document_relevance)
    , rating(document_rating) {};

std::ostream& operator<<(std::ostream& os, const Document& doc) {
    return os << "{ document_id = " << doc.id
              << ", relevance = "<< doc.relevance
              << ", rating = " << doc.rating << " }";
}