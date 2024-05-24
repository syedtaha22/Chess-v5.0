#ifndef KING_NOT_FOUND_ERROR_H
#define KING_NOT_FOUND_ERROR_H

#include <exception>
#include <string>


class KingNotFound : public std::exception {
public:
    explicit KingNotFound(int position);
    const char* what() const noexcept override;

private:
    int position;
    std::string message;
};

#endif // KING_NOT_FOUND_ERROR_H