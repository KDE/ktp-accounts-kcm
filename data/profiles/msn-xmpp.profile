<service xmlns="http://telepathy.freedesktop.org/wiki/service-profile-v1"
         id="msn-xmpp"
         type="IM"
         provider="msn-xmpp"
         manager="gabble"
         protocol="jabber"
         icon="im-msn">
  <name>Windows Live Messenger (using XMPP)</name>

  <parameters>
    <parameter name="server"  type="s" mandatory="1">xmpp.messenger.live.com</parameter>
    <parameter name="port"    type="u" mandatory="1">5222</parameter>
    <parameter name="ignore-ssl-errors" type="b" mandatory="1">1</parameter>
    <parameter name="account" type="s" mandatory="1">messenger.live.com</parameter>
  </parameters>
</service>
