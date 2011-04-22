<service xmlns="http://telepathy.freedesktop.org/wiki/service-profile-v1"
         id="facebook"
         type="IM"
         provider="facebook"
         manager="gabble"
         protocol="jabber"
         icon="im-facebook">
  <name>Facebook</name>

  <parameters>
    <parameter name="server"  type="s" mandatory="1">chat.facebook.com</parameter>
    <parameter name="port"    type="u" mandatory="1">5222</parameter>
  </parameters>

  <presences allow-others="1">
    <presence id="available" label="Online"  icon="online"/>
    <presence id="offline"   label="Offline"/>
    <presence id="away"      label="Inactive"/>
    <presence id="hidden"    disabled="1"/>
  </presences>

  <unsupported-channel-classes>
    <!-- this service doesn't support text roomlists -->
    <channel-class>
      <property name="org.freedesktop.Telepathy.Channel.TargetHandleType"
                type="u">3</property>
      <property name="org.freedesktop.Telepathy.Channel.ChannelType"
                type="s">org.freedesktop.Telepathy.Channel.Type.Text</property>
    </channel-class>
  </unsupported-channel-classes>
</service>