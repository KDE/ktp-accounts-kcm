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

  <presences allow-others="0">
    <presence id="available" label="Online"  icon="user-online" message="0"/>
    <presence id="offline"   label="Offline" icon="user-offline" message="0"/>
    <presence id="away"      label="Idle" icon="user-away" message="0"/>
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