/* -*- mode: c++; c-basic-offset:4 -*-
    commands/exportopenpgpcerttoprovidercommand.h

    This file is part of Kleopatra, the KDE keymanager
    SPDX-FileCopyrightText: 2008 Klarälvdalens Datakonsult AB
    SPDX-FileCopyrightText: 2022 Felix Tiede

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <commands/command.h>

#include <gpgme++/key.h>

namespace Kleo
{
namespace Commands
{

class ExportOpenPGPCertToProviderCommand : public Command
{
    Q_OBJECT
public:
    explicit ExportOpenPGPCertToProviderCommand(QAbstractItemView *view, KeyListController *parent);
    explicit ExportOpenPGPCertToProviderCommand(const GpgME::UserID &uid);

    ~ExportOpenPGPCertToProviderCommand() override;

    static Restrictions restrictions()
    {
        return OnlyOneKey | NeedSecretKey | MustBeOpenPGP;
    }

private Q_SLOTS:
    void wksJobResult(const GpgME::Error &, const QByteArray &, const QByteArray &);

private:
    void doStart() override;
    void doCancel() override;

    QString senderAddress() const;

    GpgME::UserID uid;
};

}
}
