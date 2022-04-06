/* -*- mode: c++; c-basic-offset:4 -*-
    utils/validation.cpp

    This file is part of Kleopatra, the KDE keymanager
    SPDX-FileCopyrightText: 2008 Klarälvdalens Datakonsult AB

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <config-kleopatra.h>

#include "validation.h"

#include <utils/multivalidator.h>

#include <KEmailAddress>

#include "kleopatra_debug.h"

#include <QRegularExpression>


using namespace Kleo;

namespace
{

template<class Validator>
class EmptyIsAcceptableValidator : public Validator
{
public:
    using Validator::Validator;

    QValidator::State validate(QString &str, int &pos) const override
    {
        if (str.trimmed().isEmpty()) {
            return QValidator::Acceptable;
        }
        return Validator::validate(str, pos);
    }
};

class EMailValidator : public QValidator
{
public:
    explicit EMailValidator(QObject *parent = nullptr) : QValidator(parent) {}

    State validate(QString &str, int &pos) const override
    {
        Q_UNUSED(pos)
        if (KEmailAddress::isValidSimpleAddress(str)) {
            return Acceptable;
        }
        return Intermediate;
    }
};

QValidator *regularExpressionValidator(Validation::Flags flags, const QString &regexp, QObject *parent)
{
    if (flags & Validation::Required) {
        return new QRegularExpressionValidator{QRegularExpression{regexp}, parent};
    } else {
        return new EmptyIsAcceptableValidator<QRegularExpressionValidator>{QRegularExpression{regexp}, parent};
    }
}

}

QValidator *Validation::email(Flags flags, QObject *parent)
{
    if (flags & Required) {
        return new EMailValidator{parent};
    } else {
        return new EmptyIsAcceptableValidator<EMailValidator>{parent};
    }
}

QValidator *Validation::email(const QString &addRX, Flags flags, QObject *parent)
{
    return new MultiValidator{email(flags), regularExpressionValidator(flags, addRX, nullptr), parent};
}

QValidator *Validation::pgpName(Flags flags, QObject *parent)
{
    // this regular expression is modeled after gnupg/g10/keygen.c:ask_user_id:
    static const auto name_rx = QRegularExpression{QStringLiteral("[^0-9<>][^<>@]{4,}")};
    if (flags & Required) {
        return new QRegularExpressionValidator{name_rx, parent};
    } else {
        return new EmptyIsAcceptableValidator<QRegularExpressionValidator>{name_rx, parent};
    }
}

QValidator *Validation::pgpName(const QString &addRX, Flags flags, QObject *parent)
{
    return new MultiValidator{pgpName(flags), regularExpressionValidator(flags, addRX, nullptr), parent};
}

QValidator *Validation::pgpComment(Flags flags, QObject *parent)
{
    // this regular expression is modeled after gnupg/g10/keygen.c:ask_user_id:
    static const auto comment_rx = QRegularExpression{QStringLiteral("[^()]*")};
    if (flags & Required) {
        return new QRegularExpressionValidator{comment_rx, parent};
    } else {
        return new EmptyIsAcceptableValidator<QRegularExpressionValidator>{comment_rx, parent};
    }
}

QValidator *Validation::pgpComment(const QString &addRX, Flags flags, QObject *parent)
{
    return new MultiValidator{pgpComment(flags), regularExpressionValidator(flags, addRX, nullptr), parent};
}
