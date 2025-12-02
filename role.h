#ifndef ROLE_H
#define ROLE_H
#include <QString>

enum class Role { Admin, RH, MobiliteExamens, Finance, Unknown };

inline Role roleFromDb(QString r) {
    r = r.trimmed().toLower();
    if (r == "admin") return Role::Admin;
    if (r == "rh") return Role::RH;
    if (r == "mobiliteexamens" || r == "mobilitéexamens" || r == "mobilite & examens" || r == "mobilité & examens")
        return Role::MobiliteExamens;
    if (r == "finance") return Role::Finance;
    return Role::Unknown;
}
#endif
