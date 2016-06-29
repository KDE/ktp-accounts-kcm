/*
 * This file is part of Kopete
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "dictionary.h"

#include <KLocalizedString>

Dictionary* Dictionary::s_self = 0;

Dictionary::Dictionary()
{
    // Set up the singleton instance
    s_self = this;

    // Parameters
    m_strings.insert(QLatin1String("password"), i18n("Password"));
    m_strings.insert(QLatin1String("account"), i18n("Account"));
    m_strings.insert(QLatin1String("priority"), i18n("Priority"));
    m_strings.insert(QLatin1String("port"), i18n("Port"));
    m_strings.insert(QLatin1String("alias"), i18n("Alias"));
    m_strings.insert(QLatin1String("register"), i18n("Register new Account"));
    m_strings.insert(QLatin1String("server"), i18n("Server Address"));
    m_strings.insert(QLatin1String("fallback-stun-server"), i18n("Fallback STUN server address"));
    m_strings.insert(QLatin1String("resource"), i18n("Resource"));
    m_strings.insert(QLatin1String("https-proxy-port"), i18n("HTTPS Proxy Port"));
    m_strings.insert(QLatin1String("require-encryption"), i18n("Require Encryption"));
    m_strings.insert(QLatin1String("old-ssl"), i18n("Old-style SSL support"));
    m_strings.insert(QLatin1String("fallback-stun-port"), i18n("Fallback STUN port"));
    m_strings.insert(QLatin1String("fallback-conference-server"), i18n("Fallback Conference Server Address"));
    m_strings.insert(QLatin1String("low-bandwidth"), i18n("Low Bandwidth Mode"));
    m_strings.insert(QLatin1String("stun-server"), i18n("STUN Server Address"));
    m_strings.insert(QLatin1String("stun-port"), i18n("STUN Port"));
    m_strings.insert(QLatin1String("fallback-socks5-proxies"), i18n("Fallback SOCKS5 Proxy Addresses"));
    m_strings.insert(QLatin1String("https-proxy-server"), i18n("HTTPS Proxy Server Address"));
    m_strings.insert(QLatin1String("ignore-ssl-errors"), i18n("Ignore SSL Errors"));
    m_strings.insert(QLatin1String("keepalive-interval"), i18n("Keepalive Interval"));

    // Protocols
    m_strings.insert(QLatin1String("aim"), i18n("AOL Instant Messenger"));
    m_strings.insert(QLatin1String("bigbrownchunx-skype-dbus"), i18n("Skype"));
    m_strings.insert(QLatin1String("facebook"), i18n("Facebook Chat"));
    m_strings.insert(QLatin1String("gadugadu"), i18n("Gadu-Gadu"));
    m_strings.insert(QLatin1String("google-talk"), i18n("Google Talk"));
    m_strings.insert(QLatin1String("groupwise"), i18n("Novell Groupwise"));
    m_strings.insert(QLatin1String("icq"), i18n("ICQ"));
    m_strings.insert(QLatin1String("skypeweb"), i18n("Skype"));
    m_strings.insert(QLatin1String("irc"), i18n("Internet Relay Chat"));
    m_strings.insert(QLatin1String("jabber"), i18n("Jabber/XMPP"));
    m_strings.insert(QLatin1String("local-xmpp"), i18n("Bonjour/Salut"));
    m_strings.insert(QLatin1String("mxit"), i18n("MXit"));
    m_strings.insert(QLatin1String("msn"), i18n("Windows Live Messenger"));
    m_strings.insert(QLatin1String("myspace"), i18n("MySpaceIM"));
    m_strings.insert(QLatin1String("qq"), i18n("Tencent QQ"));
    m_strings.insert(QLatin1String("sametime"), i18n("IBM Lotus Sametime"));
    m_strings.insert(QLatin1String("silc"), i18n("SILC"));
    m_strings.insert(QLatin1String("sip"), i18n("Session Initiation Protocol (SIP)"));
    m_strings.insert(QLatin1String("tel"), i18n("GSM Telephony"));
    m_strings.insert(QLatin1String("trepia"), i18n("Trepia"));
    m_strings.insert(QLatin1String("yahoo"), i18n("Yahoo! Messenger"));
    m_strings.insert(QLatin1String("yahoojp"), i18n("Yahoo! Messenger Japan"));
    m_strings.insert(QLatin1String("zephyr"), i18n("Zephyr"));
}

Dictionary::~Dictionary()
{
    // Delete the singleton instance of this class
    s_self = 0;
}

Dictionary *Dictionary::instance()
{
    // Construct the singleton if hasn't been already
    if (!s_self) {
        s_self = new Dictionary;
    }

    // Return the singleton instance of this class
    return s_self;
}

QString Dictionary::string(const QString &key) const
{
    return m_strings.value(key);
}

