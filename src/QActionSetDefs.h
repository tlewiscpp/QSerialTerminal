#ifndef QSERIALTERMINAL_QACTIONSETDEFS_H
#define QSERIALTERMINAL_QACTIONSETDEFS_H

#include <functional>
#include <QAction>

using QActionPtr = QAction*;

namespace std {

template <> struct hash<QActionPtr>  {
    std::size_t operator()(const QActionPtr& qActionPtr) const  {
        using std::hash;
        using std::string;

        return (hash<string>{}.operator()(qActionPtr->text().toStdString()));
    }
};

template<> struct equal_to<QActionPtr> {
    bool operator()(const QActionPtr &lhs, const QActionPtr &rhs) const {
        return lhs->text() == rhs->text();
    }
};

}

/*
class QActionHasher {
    size_t operator()(const QActionPtr &qAction) {
        return std::hash<std::string>{}.operator()(qAction->text().toStdString());
    }
};
*/


static std::function<bool(QAction *lhs, QAction *rhs)> qActionEqualTo {
        [](QAction *lhs, QAction *rhs) { return lhs->text() == rhs->text(); }
};


#endif //QSERIALTERMINAL_QACTIONSETDEFS_H
