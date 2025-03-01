/* -*- mode: c++; c-basic-offset:4 -*-
    conf/smartcardconfigpage.cpp

    This file is part of Kleopatra, the KDE keymanager
    SPDX-FileCopyrightText: 2022 g10 Code GmbH
    SPDX-FileContributor: Ingo Klöcker <dev@ingo-kloecker.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <config-kleopatra.h>

#include "smartcardconfigpage.h"

#include <Libkleo/Compat>
#include <Libkleo/ReaderPortSelection>

#include <KLocalizedString>

#include <QGpgME/CryptoConfig>
#include <QGpgME/Protocol>

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

using namespace Kleo;
using namespace Kleo::Config;
using namespace QGpgME;

class SmartCardConfigurationPage::Private
{
public:
    Private(SmartCardConfigurationPage *q);

    static CryptoConfigEntry *readerPortConfigEntry(const CryptoConfig *config = nullptr);

public:
    ReaderPortSelection *const mReaderPort;
};

SmartCardConfigurationPage::Private::Private(SmartCardConfigurationPage *qq)
    : mReaderPort{new ReaderPortSelection{qq->widget()}}
{
}

// static
CryptoConfigEntry *SmartCardConfigurationPage::Private::readerPortConfigEntry(const CryptoConfig *config)
{
    if (!config) {
        config = QGpgME::cryptoConfig();
    }
    return Kleo::getCryptoConfigEntry(config, "scdaemon", "reader-port");
}

SmartCardConfigurationPage::SmartCardConfigurationPage(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
    , d{std::make_unique<Private>(this)}
{
    auto mainLayout = new QVBoxLayout{widget()};

    {
        auto l = new QHBoxLayout{};
        l->setContentsMargins(0, 0, 0, 0);

        auto label = new QLabel{i18n("Smart card reader to use:"), widget()};
        label->setBuddy(d->mReaderPort);

        l->addWidget(label);
        l->addWidget(d->mReaderPort, 1);

        mainLayout->addLayout(l);

        connect(d->mReaderPort, &ReaderPortSelection::valueChanged, this, &SmartCardConfigurationPage::markAsChanged);
    }

    mainLayout->addStretch();

    load();
}

SmartCardConfigurationPage::~SmartCardConfigurationPage() = default;

void SmartCardConfigurationPage::load()
{
    const auto *const entry = d->readerPortConfigEntry();
    if (entry) {
        d->mReaderPort->setEnabled(!entry->isReadOnly());
        d->mReaderPort->setValue(entry->stringValue());
    } else {
        d->mReaderPort->setEnabled(false);
        d->mReaderPort->setValue(i18n("Cannot be configured with Kleopatra"));
    }
}

void SmartCardConfigurationPage::save()
{
    auto config = QGpgME::cryptoConfig();

    auto const entry = d->readerPortConfigEntry(config);
    if (entry && !entry->isReadOnly()) {
        entry->setStringValue(d->mReaderPort->value());
    }

    config->sync(true);
}

void SmartCardConfigurationPage::defaults()
{
    const auto *const entry = d->readerPortConfigEntry();
    if (entry && !entry->isReadOnly()) {
        d->mReaderPort->setValue({});
    }
}

#include "moc_smartcardconfigpage.cpp"
