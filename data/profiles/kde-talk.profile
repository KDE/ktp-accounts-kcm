<service xmlns="http://telepathy.freedesktop.org/wiki/service-profile-v1"
         id="kde-talk"
         type="IM"
         provider="kde-talk"
         manager="gabble"
         protocol="jabber"
         icon="im-kde-talk">
  <name>KDE Talk</name>

  <parameters>
    <parameter name="server"  type="s" mandatory="1">kdetalk.net</parameter>
    <parameter name="port"    type="u" mandatory="1">5222</parameter>
    <!-- This is necessary until we have a SSL handler -->
    <parameter name="ignore-ssl-errors" type="b" mandatory="1">1</parameter>
  </parameters>
</service>
