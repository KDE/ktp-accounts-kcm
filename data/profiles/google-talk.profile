<service xmlns="http://telepathy.freedesktop.org/wiki/service-profile-v1"
         id="google-talk"
         type="IM"
         provider="google"
         manager="gabble"
         protocol="jabber"
         icon="im-google-talk">
  <name>Google Talk</name>

  <parameters>
    <parameter name="server"  type="s" mandatory="1">talk.google.com</parameter>
    <parameter name="port"    type="u" mandatory="1">5223</parameter>
    <parameter name="old-ssl" type="b" mandatory="1"
               label="Use old-style SSL">1</parameter>
    <parameter name="fallback-conference-server">conference.jabber.org</parameter>
  </parameters>

  <presences allow-others="1">
    <presence id="available" label="Online"  icon="user-online"/>
    <presence id="busy"   label="Busy" icon="user-busy"/>
    <presence id="hidden" label="Invisible" icon="user-invisible"/>
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
