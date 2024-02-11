// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь: 271 =)

// Закомитьте изменения и отправьте их в свой репозиторий.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }
    
    void AddDocument(int document_id, const string& document) {
        ++document_count_;
        auto words = SplitIntoWordsNoStop(document);
        for (const auto& word : words) {
            if (word_to_document_relevance_[word].count(document_id)) {
                continue;
            }
            double tf = static_cast<double>(count(words.begin(), words.end(), word))
            / words.size();
            word_to_document_relevance_[word].insert({document_id, tf});
        }
    }
    
    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };
    
    map<string, map<int, double>> word_to_document_relevance_;
    map<int, int> document_rating_;
    set<string> stop_words_;
    int document_count_ = 0;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static bool IsMinusWord(const string& word) {
        return !word.empty() && word[0] == '-';
    }

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if (IsMinusWord(word)) {
                query.minus_words.insert(word.substr(1));
            } else {
                query.plus_words.insert(word);
            }
        }
        return query;
    }

    double CalculateIDF(const string& word) const {
        return log(static_cast<double>(document_count_)
        / word_to_document_relevance_.at(word).size());
    }

    vector<Document> FindAllDocuments(const Query& query) const {
        map<int, double> document_to_relevance;
        for (const auto& plus_word : query.plus_words) {
            if (word_to_document_relevance_.count(plus_word)) {
                for (const auto& [id, tf] : word_to_document_relevance_.at(plus_word)) {
                    double idf = CalculateIDF(plus_word);
                    document_to_relevance[id] += idf * tf;
                }
            }
        }

        for (const auto& minus_word : query.minus_words) {
            if (word_to_document_relevance_.count(minus_word)) {
                for (const auto& [id, rel] : word_to_document_relevance_.at(minus_word)) {
                    document_to_relevance.erase(id);
                }
            }
        }
        
        vector<Document> documents;
        for (const auto& [id, relevance] : document_to_relevance) {
            documents.push_back({id, relevance});
        }
        
        return documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }
    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
