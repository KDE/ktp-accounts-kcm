<service xmlns="http://telepathy.freedesktop.org/wiki/service-profile-v1"
         id="kde-talk"
         type="IM"
         provider="kde-talk"
         manager="gabble"
         protocol="jabber"
         icon="kde"> <!-- FIXME: Ask for proper icon once -->
  <name>KDE Talk</name>

  <parameters>
    <parameter name="server"  type="s" mandatory="1" />
    <parameter name="port"    type="u" mandatory="1">5222</parameter>
  </parameters>
</service>
