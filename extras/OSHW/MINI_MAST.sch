<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.5.1">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="16" fill="1" visible="no" active="no"/>
<layer number="3" name="Route3" color="17" fill="1" visible="no" active="no"/>
<layer number="4" name="Route4" color="18" fill="1" visible="no" active="no"/>
<layer number="5" name="Route5" color="19" fill="1" visible="no" active="no"/>
<layer number="6" name="Route6" color="25" fill="1" visible="no" active="no"/>
<layer number="7" name="Route7" color="26" fill="1" visible="no" active="no"/>
<layer number="8" name="Route8" color="27" fill="1" visible="no" active="no"/>
<layer number="9" name="Route9" color="28" fill="1" visible="no" active="no"/>
<layer number="10" name="Route10" color="29" fill="1" visible="no" active="no"/>
<layer number="11" name="Route11" color="30" fill="1" visible="no" active="no"/>
<layer number="12" name="Route12" color="20" fill="1" visible="no" active="no"/>
<layer number="13" name="Route13" color="21" fill="1" visible="no" active="no"/>
<layer number="14" name="Route14" color="22" fill="1" visible="no" active="no"/>
<layer number="15" name="Route15" color="23" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="SK6805-1515">
<packages>
<package name="1515">
<smd name="DIN" x="-0.48" y="-0.48" dx="0.55" dy="0.55" layer="1"/>
<smd name="5V" x="0.48" y="-0.48" dx="0.55" dy="0.55" layer="1"/>
<smd name="DOUT" x="0.48" y="0.48" dx="0.55" dy="0.55" layer="1"/>
<smd name="GND" x="-0.48" y="0.48" dx="0.55" dy="0.55" layer="1"/>
<wire x1="-0.8" y1="0.8" x2="0.8" y2="0.8" width="0.0762" layer="21"/>
<wire x1="0.8" y1="0.8" x2="0.8" y2="-0.8" width="0.0762" layer="21"/>
<wire x1="-0.8" y1="0.8" x2="-0.8" y2="-0.2" width="0.0762" layer="21"/>
<wire x1="-0.2" y1="-0.8" x2="0.8" y2="-0.8" width="0.0762" layer="21"/>
<wire x1="-0.8" y1="-0.4" x2="-0.8" y2="-0.8" width="0.0762" layer="21"/>
<wire x1="-0.8" y1="-0.8" x2="-0.4" y2="-0.8" width="0.0762" layer="21"/>
<text x="-0.835" y="0.9525" size="0.254" layer="51">&gt;NAME</text>
<wire x1="0.7" y1="1.027" x2="1.027" y2="1.027" width="0.1016" layer="21"/>
<wire x1="1.027" y1="1.027" x2="1.027" y2="0.7" width="0.1016" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="SK6805-1515">
<wire x1="0" y1="0" x2="0" y2="10.16" width="0.254" layer="94"/>
<wire x1="0" y1="10.16" x2="10.16" y2="10.16" width="0.254" layer="94"/>
<wire x1="10.16" y1="10.16" x2="10.16" y2="0" width="0.254" layer="94"/>
<wire x1="10.16" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<pin name="GND" x="5.08" y="-2.54" visible="pad" length="short" direction="pwr" rot="R90"/>
<pin name="DIN" x="-2.54" y="5.08" visible="off" length="short"/>
<pin name="DOUT" x="12.7" y="5.08" visible="off" length="short" rot="R180"/>
<pin name="5V" x="7.62" y="12.7" visible="pad" length="short" direction="pwr" rot="R270"/>
<text x="1.27" y="8.89" size="0.635" layer="94">SK6805-1515</text>
<text x="0" y="11.43" size="1.27" layer="95">&gt;NAME</text>
<text x="0.635" y="5.08" size="0.635" layer="94">DIN</text>
<text x="6.985" y="5.08" size="0.635" layer="94">DOUT</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="SK6805-1515" prefix="LED" uservalue="yes">
<gates>
<gate name="G$1" symbol="SK6805-1515" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1515">
<connects>
<connect gate="G$1" pin="5V" pad="5V"/>
<connect gate="G$1" pin="DIN" pad="DIN"/>
<connect gate="G$1" pin="DOUT" pad="DOUT"/>
<connect gate="G$1" pin="GND" pad="GND"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="pinhead" urn="urn:adsk.eagle:library:325">
<description>&lt;b&gt;Pin Header Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="2X02" urn="urn:adsk.eagle:footprint:22337/1" library_version="4">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-2.54" y1="-1.905" x2="-1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-2.54" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="0.635" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-2.54" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="-2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.905" x2="-1.905" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.54" x2="-0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="2.54" x2="0" y2="1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="1.905" x2="0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="0.635" y1="2.54" x2="1.905" y2="2.54" width="0.1524" layer="21"/>
<wire x1="1.905" y1="2.54" x2="2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="1.905" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.905" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-2.54" x2="1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-2.54" x2="-0.635" y2="-2.54" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-1.27" y="1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="1.27" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="1.27" y="1.27" drill="1.016" shape="octagon"/>
<text x="-2.54" y="3.175" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-4.445" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.524" y1="-1.524" x2="-1.016" y2="-1.016" layer="51"/>
<rectangle x1="-1.524" y1="1.016" x2="-1.016" y2="1.524" layer="51"/>
<rectangle x1="1.016" y1="1.016" x2="1.524" y2="1.524" layer="51"/>
<rectangle x1="1.016" y1="-1.524" x2="1.524" y2="-1.016" layer="51"/>
</package>
<package name="2X02/90" urn="urn:adsk.eagle:footprint:22338/1" library_version="4">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-2.54" y1="-1.905" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="6.985" x2="-1.27" y2="1.27" width="0.762" layer="21"/>
<wire x1="0" y1="-1.905" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="6.985" x2="1.27" y2="1.27" width="0.762" layer="21"/>
<pad name="2" x="-1.27" y="-3.81" drill="1.016" shape="octagon"/>
<pad name="4" x="1.27" y="-3.81" drill="1.016" shape="octagon"/>
<pad name="1" x="-1.27" y="-6.35" drill="1.016" shape="octagon"/>
<pad name="3" x="1.27" y="-6.35" drill="1.016" shape="octagon"/>
<text x="-3.175" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="4.445" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.651" y1="0.635" x2="-0.889" y2="1.143" layer="21"/>
<rectangle x1="0.889" y1="0.635" x2="1.651" y2="1.143" layer="21"/>
<rectangle x1="-1.651" y1="-2.921" x2="-0.889" y2="-1.905" layer="21"/>
<rectangle x1="0.889" y1="-2.921" x2="1.651" y2="-1.905" layer="21"/>
<rectangle x1="-1.651" y1="-5.461" x2="-0.889" y2="-4.699" layer="21"/>
<rectangle x1="-1.651" y1="-4.699" x2="-0.889" y2="-2.921" layer="51"/>
<rectangle x1="0.889" y1="-4.699" x2="1.651" y2="-2.921" layer="51"/>
<rectangle x1="0.889" y1="-5.461" x2="1.651" y2="-4.699" layer="21"/>
</package>
</packages>
<packages3d>
<package3d name="2X02" urn="urn:adsk.eagle:package:22455/2" type="model" library_version="4">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="2X02"/>
</packageinstances>
</package3d>
<package3d name="2X02/90" urn="urn:adsk.eagle:package:22456/2" type="model" library_version="4">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="2X02/90"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="PINH2X2" urn="urn:adsk.eagle:symbol:22336/1" library_version="4">
<wire x1="-8.89" y1="-2.54" x2="6.35" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="6.35" y1="-2.54" x2="6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="6.35" y1="5.08" x2="-8.89" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-8.89" y1="5.08" x2="-8.89" y2="-2.54" width="0.4064" layer="94"/>
<text x="-8.89" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-8.89" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-5.08" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot" rot="R180"/>
<pin name="3" x="-5.08" y="0" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="4" x="2.54" y="0" visible="pad" length="short" direction="pas" function="dot" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PINHD-2X2" urn="urn:adsk.eagle:component:22526/4" prefix="JP" uservalue="yes" library_version="4">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="PINH2X2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="2X02">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22455/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="2X02/90">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22456/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="LED1" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED2" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="JP1" library="pinhead" library_urn="urn:adsk.eagle:library:325" deviceset="PINHD-2X2" device="/90" package3d_urn="urn:adsk.eagle:package:22456/2"/>
<part name="JP2" library="pinhead" library_urn="urn:adsk.eagle:library:325" deviceset="PINHD-2X2" device="/90" package3d_urn="urn:adsk.eagle:package:22456/2"/>
<part name="LED3" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED4" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED5" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED6" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED7" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED8" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED9" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED10" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED11" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED12" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED13" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED14" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED15" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED16" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED17" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED18" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED19" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED20" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED21" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED22" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED23" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED24" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED25" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED26" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED27" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED28" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED29" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED30" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED31" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED32" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED33" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED34" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED35" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED36" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED37" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED38" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED39" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED40" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED41" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED42" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED43" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED44" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED45" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED46" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED47" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED48" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED49" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED50" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED51" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED52" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED53" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED54" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED55" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED56" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED57" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED58" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED59" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED60" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED61" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED62" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED63" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED64" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED65" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED66" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED67" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED68" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED69" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED70" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED71" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED72" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED73" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED74" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED75" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED76" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED77" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED78" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED79" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED80" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED81" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED82" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED83" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED84" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED85" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED86" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED87" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED88" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED89" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED90" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED91" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED92" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED93" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED94" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED95" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED96" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED97" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED98" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED99" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED100" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED101" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED102" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED103" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED104" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED105" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED106" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED107" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED108" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED109" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED110" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED111" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED112" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED113" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED114" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED115" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED116" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED117" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED118" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED119" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED120" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED121" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED122" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED123" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED124" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED125" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED126" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED127" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
<part name="LED128" library="SK6805-1515" deviceset="SK6805-1515" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="LED1" gate="G$1" x="0" y="0" smashed="yes">
<attribute name="NAME" x="0" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED2" gate="G$1" x="15.24" y="0" smashed="yes">
<attribute name="NAME" x="15.24" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="JP1" gate="A" x="-2.54" y="45.72" smashed="yes">
<attribute name="NAME" x="-11.43" y="51.435" size="1.778" layer="95"/>
<attribute name="VALUE" x="-11.43" y="40.64" size="1.778" layer="96"/>
</instance>
<instance part="JP2" gate="A" x="38.1" y="45.72" smashed="yes">
<attribute name="NAME" x="29.21" y="51.435" size="1.778" layer="95"/>
<attribute name="VALUE" x="29.21" y="40.64" size="1.778" layer="96"/>
</instance>
<instance part="LED3" gate="G$1" x="30.48" y="0" smashed="yes">
<attribute name="NAME" x="30.48" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED4" gate="G$1" x="45.72" y="0" smashed="yes">
<attribute name="NAME" x="45.72" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED5" gate="G$1" x="60.96" y="0" smashed="yes">
<attribute name="NAME" x="60.96" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED6" gate="G$1" x="76.2" y="0" smashed="yes">
<attribute name="NAME" x="76.2" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED7" gate="G$1" x="91.44" y="0" smashed="yes">
<attribute name="NAME" x="91.44" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED8" gate="G$1" x="106.68" y="0" smashed="yes">
<attribute name="NAME" x="106.68" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED9" gate="G$1" x="121.92" y="0" smashed="yes">
<attribute name="NAME" x="121.92" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED10" gate="G$1" x="137.16" y="0" smashed="yes">
<attribute name="NAME" x="137.16" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED11" gate="G$1" x="152.4" y="0" smashed="yes">
<attribute name="NAME" x="152.4" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED12" gate="G$1" x="167.64" y="0" smashed="yes">
<attribute name="NAME" x="167.64" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED13" gate="G$1" x="182.88" y="0" smashed="yes">
<attribute name="NAME" x="182.88" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED14" gate="G$1" x="198.12" y="0" smashed="yes">
<attribute name="NAME" x="198.12" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED15" gate="G$1" x="213.36" y="0" smashed="yes">
<attribute name="NAME" x="213.36" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED16" gate="G$1" x="228.6" y="0" smashed="yes">
<attribute name="NAME" x="228.6" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED17" gate="G$1" x="243.84" y="0" smashed="yes">
<attribute name="NAME" x="243.84" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED18" gate="G$1" x="259.08" y="0" smashed="yes">
<attribute name="NAME" x="259.08" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED19" gate="G$1" x="274.32" y="0" smashed="yes">
<attribute name="NAME" x="274.32" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED20" gate="G$1" x="289.56" y="0" smashed="yes">
<attribute name="NAME" x="289.56" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED21" gate="G$1" x="304.8" y="0" smashed="yes">
<attribute name="NAME" x="304.8" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED22" gate="G$1" x="320.04" y="0" smashed="yes">
<attribute name="NAME" x="320.04" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED23" gate="G$1" x="335.28" y="0" smashed="yes">
<attribute name="NAME" x="335.28" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED24" gate="G$1" x="350.52" y="0" smashed="yes">
<attribute name="NAME" x="350.52" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED25" gate="G$1" x="365.76" y="0" smashed="yes">
<attribute name="NAME" x="365.76" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED26" gate="G$1" x="381" y="0" smashed="yes">
<attribute name="NAME" x="381" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED27" gate="G$1" x="396.24" y="0" smashed="yes">
<attribute name="NAME" x="396.24" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED28" gate="G$1" x="411.48" y="0" smashed="yes">
<attribute name="NAME" x="411.48" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED29" gate="G$1" x="426.72" y="0" smashed="yes">
<attribute name="NAME" x="426.72" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED30" gate="G$1" x="441.96" y="0" smashed="yes">
<attribute name="NAME" x="441.96" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED31" gate="G$1" x="457.2" y="0" smashed="yes">
<attribute name="NAME" x="457.2" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED32" gate="G$1" x="472.44" y="0" smashed="yes">
<attribute name="NAME" x="472.44" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED33" gate="G$1" x="487.68" y="0" smashed="yes">
<attribute name="NAME" x="487.68" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED34" gate="G$1" x="502.92" y="0" smashed="yes">
<attribute name="NAME" x="502.92" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED35" gate="G$1" x="518.16" y="0" smashed="yes">
<attribute name="NAME" x="518.16" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED36" gate="G$1" x="533.4" y="0" smashed="yes">
<attribute name="NAME" x="533.4" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED37" gate="G$1" x="548.64" y="0" smashed="yes">
<attribute name="NAME" x="548.64" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED38" gate="G$1" x="563.88" y="0" smashed="yes">
<attribute name="NAME" x="563.88" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED39" gate="G$1" x="579.12" y="0" smashed="yes">
<attribute name="NAME" x="579.12" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED40" gate="G$1" x="594.36" y="0" smashed="yes">
<attribute name="NAME" x="594.36" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED41" gate="G$1" x="609.6" y="0" smashed="yes">
<attribute name="NAME" x="609.6" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED42" gate="G$1" x="624.84" y="0" smashed="yes">
<attribute name="NAME" x="624.84" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED43" gate="G$1" x="640.08" y="0" smashed="yes">
<attribute name="NAME" x="640.08" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED44" gate="G$1" x="655.32" y="0" smashed="yes">
<attribute name="NAME" x="655.32" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED45" gate="G$1" x="670.56" y="0" smashed="yes">
<attribute name="NAME" x="670.56" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED46" gate="G$1" x="685.8" y="0" smashed="yes">
<attribute name="NAME" x="685.8" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED47" gate="G$1" x="701.04" y="0" smashed="yes">
<attribute name="NAME" x="701.04" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED48" gate="G$1" x="716.28" y="0" smashed="yes">
<attribute name="NAME" x="716.28" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED49" gate="G$1" x="731.52" y="0" smashed="yes">
<attribute name="NAME" x="731.52" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED50" gate="G$1" x="746.76" y="0" smashed="yes">
<attribute name="NAME" x="746.76" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED51" gate="G$1" x="762" y="0" smashed="yes">
<attribute name="NAME" x="762" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED52" gate="G$1" x="777.24" y="0" smashed="yes">
<attribute name="NAME" x="777.24" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED53" gate="G$1" x="792.48" y="0" smashed="yes">
<attribute name="NAME" x="792.48" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED54" gate="G$1" x="807.72" y="0" smashed="yes">
<attribute name="NAME" x="807.72" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED55" gate="G$1" x="822.96" y="0" smashed="yes">
<attribute name="NAME" x="822.96" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED56" gate="G$1" x="838.2" y="0" smashed="yes">
<attribute name="NAME" x="838.2" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED57" gate="G$1" x="853.44" y="0" smashed="yes">
<attribute name="NAME" x="853.44" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED58" gate="G$1" x="868.68" y="0" smashed="yes">
<attribute name="NAME" x="868.68" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED59" gate="G$1" x="883.92" y="0" smashed="yes">
<attribute name="NAME" x="883.92" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED60" gate="G$1" x="899.16" y="0" smashed="yes">
<attribute name="NAME" x="899.16" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED61" gate="G$1" x="914.4" y="0" smashed="yes">
<attribute name="NAME" x="914.4" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED62" gate="G$1" x="929.64" y="0" smashed="yes">
<attribute name="NAME" x="929.64" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED63" gate="G$1" x="944.88" y="0" smashed="yes">
<attribute name="NAME" x="944.88" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED64" gate="G$1" x="960.12" y="0" smashed="yes">
<attribute name="NAME" x="960.12" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED65" gate="G$1" x="975.36" y="0" smashed="yes">
<attribute name="NAME" x="975.36" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED66" gate="G$1" x="990.6" y="0" smashed="yes">
<attribute name="NAME" x="990.6" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED67" gate="G$1" x="1005.84" y="0" smashed="yes">
<attribute name="NAME" x="1005.84" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED68" gate="G$1" x="1021.08" y="0" smashed="yes">
<attribute name="NAME" x="1021.08" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED69" gate="G$1" x="1036.32" y="0" smashed="yes">
<attribute name="NAME" x="1036.32" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED70" gate="G$1" x="1051.56" y="0" smashed="yes">
<attribute name="NAME" x="1051.56" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED71" gate="G$1" x="1066.8" y="0" smashed="yes">
<attribute name="NAME" x="1066.8" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED72" gate="G$1" x="1082.04" y="0" smashed="yes">
<attribute name="NAME" x="1082.04" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED73" gate="G$1" x="1097.28" y="0" smashed="yes">
<attribute name="NAME" x="1097.28" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED74" gate="G$1" x="1112.52" y="0" smashed="yes">
<attribute name="NAME" x="1112.52" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED75" gate="G$1" x="1127.76" y="0" smashed="yes">
<attribute name="NAME" x="1127.76" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED76" gate="G$1" x="1143" y="0" smashed="yes">
<attribute name="NAME" x="1143" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED77" gate="G$1" x="1158.24" y="0" smashed="yes">
<attribute name="NAME" x="1158.24" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED78" gate="G$1" x="1173.48" y="0" smashed="yes">
<attribute name="NAME" x="1173.48" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED79" gate="G$1" x="1188.72" y="0" smashed="yes">
<attribute name="NAME" x="1188.72" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED80" gate="G$1" x="1203.96" y="0" smashed="yes">
<attribute name="NAME" x="1203.96" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED81" gate="G$1" x="1219.2" y="0" smashed="yes">
<attribute name="NAME" x="1219.2" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED82" gate="G$1" x="1234.44" y="0" smashed="yes">
<attribute name="NAME" x="1234.44" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED83" gate="G$1" x="1249.68" y="0" smashed="yes">
<attribute name="NAME" x="1249.68" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED84" gate="G$1" x="1264.92" y="0" smashed="yes">
<attribute name="NAME" x="1264.92" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED85" gate="G$1" x="1280.16" y="0" smashed="yes">
<attribute name="NAME" x="1280.16" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED86" gate="G$1" x="1295.4" y="0" smashed="yes">
<attribute name="NAME" x="1295.4" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED87" gate="G$1" x="1310.64" y="0" smashed="yes">
<attribute name="NAME" x="1310.64" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED88" gate="G$1" x="1325.88" y="0" smashed="yes">
<attribute name="NAME" x="1325.88" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED89" gate="G$1" x="1341.12" y="0" smashed="yes">
<attribute name="NAME" x="1341.12" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED90" gate="G$1" x="1356.36" y="0" smashed="yes">
<attribute name="NAME" x="1356.36" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED91" gate="G$1" x="1371.6" y="0" smashed="yes">
<attribute name="NAME" x="1371.6" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED92" gate="G$1" x="1386.84" y="0" smashed="yes">
<attribute name="NAME" x="1386.84" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED93" gate="G$1" x="1402.08" y="0" smashed="yes">
<attribute name="NAME" x="1402.08" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED94" gate="G$1" x="1417.32" y="0" smashed="yes">
<attribute name="NAME" x="1417.32" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED95" gate="G$1" x="1432.56" y="0" smashed="yes">
<attribute name="NAME" x="1432.56" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED96" gate="G$1" x="1447.8" y="0" smashed="yes">
<attribute name="NAME" x="1447.8" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED97" gate="G$1" x="1463.04" y="0" smashed="yes">
<attribute name="NAME" x="1463.04" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED98" gate="G$1" x="1478.28" y="0" smashed="yes">
<attribute name="NAME" x="1478.28" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED99" gate="G$1" x="1493.52" y="0" smashed="yes">
<attribute name="NAME" x="1493.52" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED100" gate="G$1" x="1508.76" y="0" smashed="yes">
<attribute name="NAME" x="1508.76" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED101" gate="G$1" x="1524" y="0" smashed="yes">
<attribute name="NAME" x="1524" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED102" gate="G$1" x="1539.24" y="0" smashed="yes">
<attribute name="NAME" x="1539.24" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED103" gate="G$1" x="1554.48" y="0" smashed="yes">
<attribute name="NAME" x="1554.48" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED104" gate="G$1" x="1569.72" y="0" smashed="yes">
<attribute name="NAME" x="1569.72" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED105" gate="G$1" x="1584.96" y="0" smashed="yes">
<attribute name="NAME" x="1584.96" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED106" gate="G$1" x="1600.2" y="0" smashed="yes">
<attribute name="NAME" x="1600.2" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED107" gate="G$1" x="1615.44" y="0" smashed="yes">
<attribute name="NAME" x="1615.44" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED108" gate="G$1" x="1630.68" y="0" smashed="yes">
<attribute name="NAME" x="1630.68" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED109" gate="G$1" x="1645.92" y="0" smashed="yes">
<attribute name="NAME" x="1645.92" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED110" gate="G$1" x="1661.16" y="0" smashed="yes">
<attribute name="NAME" x="1661.16" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED111" gate="G$1" x="1676.4" y="0" smashed="yes">
<attribute name="NAME" x="1676.4" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED112" gate="G$1" x="1691.64" y="0" smashed="yes">
<attribute name="NAME" x="1691.64" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED113" gate="G$1" x="1706.88" y="0" smashed="yes">
<attribute name="NAME" x="1706.88" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED114" gate="G$1" x="1722.12" y="0" smashed="yes">
<attribute name="NAME" x="1722.12" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED115" gate="G$1" x="1737.36" y="0" smashed="yes">
<attribute name="NAME" x="1737.36" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED116" gate="G$1" x="1752.6" y="0" smashed="yes">
<attribute name="NAME" x="1752.6" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED117" gate="G$1" x="1767.84" y="0" smashed="yes">
<attribute name="NAME" x="1767.84" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED118" gate="G$1" x="1783.08" y="0" smashed="yes">
<attribute name="NAME" x="1783.08" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED119" gate="G$1" x="1798.32" y="0" smashed="yes">
<attribute name="NAME" x="1798.32" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED120" gate="G$1" x="1813.56" y="0" smashed="yes">
<attribute name="NAME" x="1813.56" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED121" gate="G$1" x="1828.8" y="0" smashed="yes">
<attribute name="NAME" x="1828.8" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED122" gate="G$1" x="1844.04" y="0" smashed="yes">
<attribute name="NAME" x="1844.04" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED123" gate="G$1" x="1859.28" y="0" smashed="yes">
<attribute name="NAME" x="1859.28" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED124" gate="G$1" x="1874.52" y="0" smashed="yes">
<attribute name="NAME" x="1874.52" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED125" gate="G$1" x="1889.76" y="0" smashed="yes">
<attribute name="NAME" x="1889.76" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED126" gate="G$1" x="1905" y="0" smashed="yes">
<attribute name="NAME" x="1905" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED127" gate="G$1" x="1920.24" y="0" smashed="yes">
<attribute name="NAME" x="1920.24" y="11.43" size="1.27" layer="95"/>
</instance>
<instance part="LED128" gate="G$1" x="1935.48" y="0" smashed="yes">
<attribute name="NAME" x="1935.48" y="11.43" size="1.27" layer="95"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="5V" class="0">
<segment>
<pinref part="LED1" gate="G$1" pin="5V"/>
<wire x1="7.62" y1="12.7" x2="7.62" y2="15.24" width="0.1524" layer="91"/>
<label x="7.62" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED2" gate="G$1" pin="5V"/>
<wire x1="22.86" y1="12.7" x2="22.86" y2="15.24" width="0.1524" layer="91"/>
<label x="22.86" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="4"/>
<wire x1="0" y1="45.72" x2="5.08" y2="45.72" width="0.1524" layer="91"/>
<wire x1="5.08" y1="45.72" x2="5.08" y2="43.18" width="0.1524" layer="91"/>
<wire x1="5.08" y1="43.18" x2="7.62" y2="43.18" width="0.1524" layer="91"/>
<label x="7.62" y="43.18" size="1.778" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="LED3" gate="G$1" pin="5V"/>
<wire x1="38.1" y1="12.7" x2="38.1" y2="15.24" width="0.1524" layer="91"/>
<label x="38.1" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED4" gate="G$1" pin="5V"/>
<wire x1="53.34" y1="12.7" x2="53.34" y2="15.24" width="0.1524" layer="91"/>
<label x="53.34" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED5" gate="G$1" pin="5V"/>
<wire x1="68.58" y1="12.7" x2="68.58" y2="15.24" width="0.1524" layer="91"/>
<label x="68.58" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED6" gate="G$1" pin="5V"/>
<wire x1="83.82" y1="12.7" x2="83.82" y2="15.24" width="0.1524" layer="91"/>
<label x="83.82" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED7" gate="G$1" pin="5V"/>
<wire x1="99.06" y1="12.7" x2="99.06" y2="15.24" width="0.1524" layer="91"/>
<label x="99.06" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED8" gate="G$1" pin="5V"/>
<wire x1="114.3" y1="12.7" x2="114.3" y2="15.24" width="0.1524" layer="91"/>
<label x="114.3" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED9" gate="G$1" pin="5V"/>
<wire x1="129.54" y1="12.7" x2="129.54" y2="15.24" width="0.1524" layer="91"/>
<label x="129.54" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED10" gate="G$1" pin="5V"/>
<wire x1="144.78" y1="12.7" x2="144.78" y2="15.24" width="0.1524" layer="91"/>
<label x="144.78" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED11" gate="G$1" pin="5V"/>
<wire x1="160.02" y1="12.7" x2="160.02" y2="15.24" width="0.1524" layer="91"/>
<label x="160.02" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED12" gate="G$1" pin="5V"/>
<wire x1="175.26" y1="12.7" x2="175.26" y2="15.24" width="0.1524" layer="91"/>
<label x="175.26" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED13" gate="G$1" pin="5V"/>
<wire x1="190.5" y1="12.7" x2="190.5" y2="15.24" width="0.1524" layer="91"/>
<label x="190.5" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED14" gate="G$1" pin="5V"/>
<wire x1="205.74" y1="12.7" x2="205.74" y2="15.24" width="0.1524" layer="91"/>
<label x="205.74" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED15" gate="G$1" pin="5V"/>
<wire x1="220.98" y1="12.7" x2="220.98" y2="15.24" width="0.1524" layer="91"/>
<label x="220.98" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED16" gate="G$1" pin="5V"/>
<wire x1="236.22" y1="12.7" x2="236.22" y2="15.24" width="0.1524" layer="91"/>
<label x="236.22" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED17" gate="G$1" pin="5V"/>
<wire x1="251.46" y1="12.7" x2="251.46" y2="15.24" width="0.1524" layer="91"/>
<label x="251.46" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED18" gate="G$1" pin="5V"/>
<wire x1="266.7" y1="12.7" x2="266.7" y2="15.24" width="0.1524" layer="91"/>
<label x="266.7" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED19" gate="G$1" pin="5V"/>
<wire x1="281.94" y1="12.7" x2="281.94" y2="15.24" width="0.1524" layer="91"/>
<label x="281.94" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED20" gate="G$1" pin="5V"/>
<wire x1="297.18" y1="12.7" x2="297.18" y2="15.24" width="0.1524" layer="91"/>
<label x="297.18" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED21" gate="G$1" pin="5V"/>
<wire x1="312.42" y1="12.7" x2="312.42" y2="15.24" width="0.1524" layer="91"/>
<label x="312.42" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED22" gate="G$1" pin="5V"/>
<wire x1="327.66" y1="12.7" x2="327.66" y2="15.24" width="0.1524" layer="91"/>
<label x="327.66" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED23" gate="G$1" pin="5V"/>
<wire x1="342.9" y1="12.7" x2="342.9" y2="15.24" width="0.1524" layer="91"/>
<label x="342.9" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED24" gate="G$1" pin="5V"/>
<wire x1="358.14" y1="12.7" x2="358.14" y2="15.24" width="0.1524" layer="91"/>
<label x="358.14" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED25" gate="G$1" pin="5V"/>
<wire x1="373.38" y1="12.7" x2="373.38" y2="15.24" width="0.1524" layer="91"/>
<label x="373.38" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED26" gate="G$1" pin="5V"/>
<wire x1="388.62" y1="12.7" x2="388.62" y2="15.24" width="0.1524" layer="91"/>
<label x="388.62" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED27" gate="G$1" pin="5V"/>
<wire x1="403.86" y1="12.7" x2="403.86" y2="15.24" width="0.1524" layer="91"/>
<label x="403.86" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED28" gate="G$1" pin="5V"/>
<wire x1="419.1" y1="12.7" x2="419.1" y2="15.24" width="0.1524" layer="91"/>
<label x="419.1" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED29" gate="G$1" pin="5V"/>
<wire x1="434.34" y1="12.7" x2="434.34" y2="15.24" width="0.1524" layer="91"/>
<label x="434.34" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED30" gate="G$1" pin="5V"/>
<wire x1="449.58" y1="12.7" x2="449.58" y2="15.24" width="0.1524" layer="91"/>
<label x="449.58" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED31" gate="G$1" pin="5V"/>
<wire x1="464.82" y1="12.7" x2="464.82" y2="15.24" width="0.1524" layer="91"/>
<label x="464.82" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED32" gate="G$1" pin="5V"/>
<wire x1="480.06" y1="12.7" x2="480.06" y2="15.24" width="0.1524" layer="91"/>
<label x="480.06" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED33" gate="G$1" pin="5V"/>
<wire x1="495.3" y1="12.7" x2="495.3" y2="15.24" width="0.1524" layer="91"/>
<label x="495.3" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED34" gate="G$1" pin="5V"/>
<wire x1="510.54" y1="12.7" x2="510.54" y2="15.24" width="0.1524" layer="91"/>
<label x="510.54" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED35" gate="G$1" pin="5V"/>
<wire x1="525.78" y1="12.7" x2="525.78" y2="15.24" width="0.1524" layer="91"/>
<label x="525.78" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED36" gate="G$1" pin="5V"/>
<wire x1="541.02" y1="12.7" x2="541.02" y2="15.24" width="0.1524" layer="91"/>
<label x="541.02" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED37" gate="G$1" pin="5V"/>
<wire x1="556.26" y1="12.7" x2="556.26" y2="15.24" width="0.1524" layer="91"/>
<label x="556.26" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED38" gate="G$1" pin="5V"/>
<wire x1="571.5" y1="12.7" x2="571.5" y2="15.24" width="0.1524" layer="91"/>
<label x="571.5" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED39" gate="G$1" pin="5V"/>
<wire x1="586.74" y1="12.7" x2="586.74" y2="15.24" width="0.1524" layer="91"/>
<label x="586.74" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED40" gate="G$1" pin="5V"/>
<wire x1="601.98" y1="12.7" x2="601.98" y2="15.24" width="0.1524" layer="91"/>
<label x="601.98" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED41" gate="G$1" pin="5V"/>
<wire x1="617.22" y1="12.7" x2="617.22" y2="15.24" width="0.1524" layer="91"/>
<label x="617.22" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED42" gate="G$1" pin="5V"/>
<wire x1="632.46" y1="12.7" x2="632.46" y2="15.24" width="0.1524" layer="91"/>
<label x="632.46" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED43" gate="G$1" pin="5V"/>
<wire x1="647.7" y1="12.7" x2="647.7" y2="15.24" width="0.1524" layer="91"/>
<label x="647.7" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED44" gate="G$1" pin="5V"/>
<wire x1="662.94" y1="12.7" x2="662.94" y2="15.24" width="0.1524" layer="91"/>
<label x="662.94" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED45" gate="G$1" pin="5V"/>
<wire x1="678.18" y1="12.7" x2="678.18" y2="15.24" width="0.1524" layer="91"/>
<label x="678.18" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED46" gate="G$1" pin="5V"/>
<wire x1="693.42" y1="12.7" x2="693.42" y2="15.24" width="0.1524" layer="91"/>
<label x="693.42" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED47" gate="G$1" pin="5V"/>
<wire x1="708.66" y1="12.7" x2="708.66" y2="15.24" width="0.1524" layer="91"/>
<label x="708.66" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED48" gate="G$1" pin="5V"/>
<wire x1="723.9" y1="12.7" x2="723.9" y2="15.24" width="0.1524" layer="91"/>
<label x="723.9" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED49" gate="G$1" pin="5V"/>
<wire x1="739.14" y1="12.7" x2="739.14" y2="15.24" width="0.1524" layer="91"/>
<label x="739.14" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED50" gate="G$1" pin="5V"/>
<wire x1="754.38" y1="12.7" x2="754.38" y2="15.24" width="0.1524" layer="91"/>
<label x="754.38" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED51" gate="G$1" pin="5V"/>
<wire x1="769.62" y1="12.7" x2="769.62" y2="15.24" width="0.1524" layer="91"/>
<label x="769.62" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED52" gate="G$1" pin="5V"/>
<wire x1="784.86" y1="12.7" x2="784.86" y2="15.24" width="0.1524" layer="91"/>
<label x="784.86" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED53" gate="G$1" pin="5V"/>
<wire x1="800.1" y1="12.7" x2="800.1" y2="15.24" width="0.1524" layer="91"/>
<label x="800.1" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED54" gate="G$1" pin="5V"/>
<wire x1="815.34" y1="12.7" x2="815.34" y2="15.24" width="0.1524" layer="91"/>
<label x="815.34" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED55" gate="G$1" pin="5V"/>
<wire x1="830.58" y1="12.7" x2="830.58" y2="15.24" width="0.1524" layer="91"/>
<label x="830.58" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED56" gate="G$1" pin="5V"/>
<wire x1="845.82" y1="12.7" x2="845.82" y2="15.24" width="0.1524" layer="91"/>
<label x="845.82" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED57" gate="G$1" pin="5V"/>
<wire x1="861.06" y1="12.7" x2="861.06" y2="15.24" width="0.1524" layer="91"/>
<label x="861.06" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED58" gate="G$1" pin="5V"/>
<wire x1="876.3" y1="12.7" x2="876.3" y2="15.24" width="0.1524" layer="91"/>
<label x="876.3" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED59" gate="G$1" pin="5V"/>
<wire x1="891.54" y1="12.7" x2="891.54" y2="15.24" width="0.1524" layer="91"/>
<label x="891.54" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED60" gate="G$1" pin="5V"/>
<wire x1="906.78" y1="12.7" x2="906.78" y2="15.24" width="0.1524" layer="91"/>
<label x="906.78" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED61" gate="G$1" pin="5V"/>
<wire x1="922.02" y1="12.7" x2="922.02" y2="15.24" width="0.1524" layer="91"/>
<label x="922.02" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED62" gate="G$1" pin="5V"/>
<wire x1="937.26" y1="12.7" x2="937.26" y2="15.24" width="0.1524" layer="91"/>
<label x="937.26" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED63" gate="G$1" pin="5V"/>
<wire x1="952.5" y1="12.7" x2="952.5" y2="15.24" width="0.1524" layer="91"/>
<label x="952.5" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED64" gate="G$1" pin="5V"/>
<wire x1="967.74" y1="12.7" x2="967.74" y2="15.24" width="0.1524" layer="91"/>
<label x="967.74" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED65" gate="G$1" pin="5V"/>
<wire x1="982.98" y1="12.7" x2="982.98" y2="15.24" width="0.1524" layer="91"/>
<label x="982.98" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED66" gate="G$1" pin="5V"/>
<wire x1="998.22" y1="12.7" x2="998.22" y2="15.24" width="0.1524" layer="91"/>
<label x="998.22" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED67" gate="G$1" pin="5V"/>
<wire x1="1013.46" y1="12.7" x2="1013.46" y2="15.24" width="0.1524" layer="91"/>
<label x="1013.46" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED68" gate="G$1" pin="5V"/>
<wire x1="1028.7" y1="12.7" x2="1028.7" y2="15.24" width="0.1524" layer="91"/>
<label x="1028.7" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED69" gate="G$1" pin="5V"/>
<wire x1="1043.94" y1="12.7" x2="1043.94" y2="15.24" width="0.1524" layer="91"/>
<label x="1043.94" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED70" gate="G$1" pin="5V"/>
<wire x1="1059.18" y1="12.7" x2="1059.18" y2="15.24" width="0.1524" layer="91"/>
<label x="1059.18" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED71" gate="G$1" pin="5V"/>
<wire x1="1074.42" y1="12.7" x2="1074.42" y2="15.24" width="0.1524" layer="91"/>
<label x="1074.42" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED72" gate="G$1" pin="5V"/>
<wire x1="1089.66" y1="12.7" x2="1089.66" y2="15.24" width="0.1524" layer="91"/>
<label x="1089.66" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED73" gate="G$1" pin="5V"/>
<wire x1="1104.9" y1="12.7" x2="1104.9" y2="15.24" width="0.1524" layer="91"/>
<label x="1104.9" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED74" gate="G$1" pin="5V"/>
<wire x1="1120.14" y1="12.7" x2="1120.14" y2="15.24" width="0.1524" layer="91"/>
<label x="1120.14" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED75" gate="G$1" pin="5V"/>
<wire x1="1135.38" y1="12.7" x2="1135.38" y2="15.24" width="0.1524" layer="91"/>
<label x="1135.38" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED76" gate="G$1" pin="5V"/>
<wire x1="1150.62" y1="12.7" x2="1150.62" y2="15.24" width="0.1524" layer="91"/>
<label x="1150.62" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED77" gate="G$1" pin="5V"/>
<wire x1="1165.86" y1="12.7" x2="1165.86" y2="15.24" width="0.1524" layer="91"/>
<label x="1165.86" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED78" gate="G$1" pin="5V"/>
<wire x1="1181.1" y1="12.7" x2="1181.1" y2="15.24" width="0.1524" layer="91"/>
<label x="1181.1" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED79" gate="G$1" pin="5V"/>
<wire x1="1196.34" y1="12.7" x2="1196.34" y2="15.24" width="0.1524" layer="91"/>
<label x="1196.34" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED80" gate="G$1" pin="5V"/>
<wire x1="1211.58" y1="12.7" x2="1211.58" y2="15.24" width="0.1524" layer="91"/>
<label x="1211.58" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED81" gate="G$1" pin="5V"/>
<wire x1="1226.82" y1="12.7" x2="1226.82" y2="15.24" width="0.1524" layer="91"/>
<label x="1226.82" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED82" gate="G$1" pin="5V"/>
<wire x1="1242.06" y1="12.7" x2="1242.06" y2="15.24" width="0.1524" layer="91"/>
<label x="1242.06" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED83" gate="G$1" pin="5V"/>
<wire x1="1257.3" y1="12.7" x2="1257.3" y2="15.24" width="0.1524" layer="91"/>
<label x="1257.3" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED84" gate="G$1" pin="5V"/>
<wire x1="1272.54" y1="12.7" x2="1272.54" y2="15.24" width="0.1524" layer="91"/>
<label x="1272.54" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED85" gate="G$1" pin="5V"/>
<wire x1="1287.78" y1="12.7" x2="1287.78" y2="15.24" width="0.1524" layer="91"/>
<label x="1287.78" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED86" gate="G$1" pin="5V"/>
<wire x1="1303.02" y1="12.7" x2="1303.02" y2="15.24" width="0.1524" layer="91"/>
<label x="1303.02" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED87" gate="G$1" pin="5V"/>
<wire x1="1318.26" y1="12.7" x2="1318.26" y2="15.24" width="0.1524" layer="91"/>
<label x="1318.26" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED88" gate="G$1" pin="5V"/>
<wire x1="1333.5" y1="12.7" x2="1333.5" y2="15.24" width="0.1524" layer="91"/>
<label x="1333.5" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED89" gate="G$1" pin="5V"/>
<wire x1="1348.74" y1="12.7" x2="1348.74" y2="15.24" width="0.1524" layer="91"/>
<label x="1348.74" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED90" gate="G$1" pin="5V"/>
<wire x1="1363.98" y1="12.7" x2="1363.98" y2="15.24" width="0.1524" layer="91"/>
<label x="1363.98" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED91" gate="G$1" pin="5V"/>
<wire x1="1379.22" y1="12.7" x2="1379.22" y2="15.24" width="0.1524" layer="91"/>
<label x="1379.22" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED92" gate="G$1" pin="5V"/>
<wire x1="1394.46" y1="12.7" x2="1394.46" y2="15.24" width="0.1524" layer="91"/>
<label x="1394.46" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED93" gate="G$1" pin="5V"/>
<wire x1="1409.7" y1="12.7" x2="1409.7" y2="15.24" width="0.1524" layer="91"/>
<label x="1409.7" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED94" gate="G$1" pin="5V"/>
<wire x1="1424.94" y1="12.7" x2="1424.94" y2="15.24" width="0.1524" layer="91"/>
<label x="1424.94" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED95" gate="G$1" pin="5V"/>
<wire x1="1440.18" y1="12.7" x2="1440.18" y2="15.24" width="0.1524" layer="91"/>
<label x="1440.18" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED96" gate="G$1" pin="5V"/>
<wire x1="1455.42" y1="12.7" x2="1455.42" y2="15.24" width="0.1524" layer="91"/>
<label x="1455.42" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED97" gate="G$1" pin="5V"/>
<wire x1="1470.66" y1="12.7" x2="1470.66" y2="15.24" width="0.1524" layer="91"/>
<label x="1470.66" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED98" gate="G$1" pin="5V"/>
<wire x1="1485.9" y1="12.7" x2="1485.9" y2="15.24" width="0.1524" layer="91"/>
<label x="1485.9" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED99" gate="G$1" pin="5V"/>
<wire x1="1501.14" y1="12.7" x2="1501.14" y2="15.24" width="0.1524" layer="91"/>
<label x="1501.14" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED100" gate="G$1" pin="5V"/>
<wire x1="1516.38" y1="12.7" x2="1516.38" y2="15.24" width="0.1524" layer="91"/>
<label x="1516.38" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED101" gate="G$1" pin="5V"/>
<wire x1="1531.62" y1="12.7" x2="1531.62" y2="15.24" width="0.1524" layer="91"/>
<label x="1531.62" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED102" gate="G$1" pin="5V"/>
<wire x1="1546.86" y1="12.7" x2="1546.86" y2="15.24" width="0.1524" layer="91"/>
<label x="1546.86" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED103" gate="G$1" pin="5V"/>
<wire x1="1562.1" y1="12.7" x2="1562.1" y2="15.24" width="0.1524" layer="91"/>
<label x="1562.1" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED104" gate="G$1" pin="5V"/>
<wire x1="1577.34" y1="12.7" x2="1577.34" y2="15.24" width="0.1524" layer="91"/>
<label x="1577.34" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED105" gate="G$1" pin="5V"/>
<wire x1="1592.58" y1="12.7" x2="1592.58" y2="15.24" width="0.1524" layer="91"/>
<label x="1592.58" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED106" gate="G$1" pin="5V"/>
<wire x1="1607.82" y1="12.7" x2="1607.82" y2="15.24" width="0.1524" layer="91"/>
<label x="1607.82" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED107" gate="G$1" pin="5V"/>
<wire x1="1623.06" y1="12.7" x2="1623.06" y2="15.24" width="0.1524" layer="91"/>
<label x="1623.06" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED108" gate="G$1" pin="5V"/>
<wire x1="1638.3" y1="12.7" x2="1638.3" y2="15.24" width="0.1524" layer="91"/>
<label x="1638.3" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED109" gate="G$1" pin="5V"/>
<wire x1="1653.54" y1="12.7" x2="1653.54" y2="15.24" width="0.1524" layer="91"/>
<label x="1653.54" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED110" gate="G$1" pin="5V"/>
<wire x1="1668.78" y1="12.7" x2="1668.78" y2="15.24" width="0.1524" layer="91"/>
<label x="1668.78" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED111" gate="G$1" pin="5V"/>
<wire x1="1684.02" y1="12.7" x2="1684.02" y2="15.24" width="0.1524" layer="91"/>
<label x="1684.02" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED112" gate="G$1" pin="5V"/>
<wire x1="1699.26" y1="12.7" x2="1699.26" y2="15.24" width="0.1524" layer="91"/>
<label x="1699.26" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED113" gate="G$1" pin="5V"/>
<wire x1="1714.5" y1="12.7" x2="1714.5" y2="15.24" width="0.1524" layer="91"/>
<label x="1714.5" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED114" gate="G$1" pin="5V"/>
<wire x1="1729.74" y1="12.7" x2="1729.74" y2="15.24" width="0.1524" layer="91"/>
<label x="1729.74" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED115" gate="G$1" pin="5V"/>
<wire x1="1744.98" y1="12.7" x2="1744.98" y2="15.24" width="0.1524" layer="91"/>
<label x="1744.98" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED116" gate="G$1" pin="5V"/>
<wire x1="1760.22" y1="12.7" x2="1760.22" y2="15.24" width="0.1524" layer="91"/>
<label x="1760.22" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED117" gate="G$1" pin="5V"/>
<wire x1="1775.46" y1="12.7" x2="1775.46" y2="15.24" width="0.1524" layer="91"/>
<label x="1775.46" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED118" gate="G$1" pin="5V"/>
<wire x1="1790.7" y1="12.7" x2="1790.7" y2="15.24" width="0.1524" layer="91"/>
<label x="1790.7" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED119" gate="G$1" pin="5V"/>
<wire x1="1805.94" y1="12.7" x2="1805.94" y2="15.24" width="0.1524" layer="91"/>
<label x="1805.94" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED120" gate="G$1" pin="5V"/>
<wire x1="1821.18" y1="12.7" x2="1821.18" y2="15.24" width="0.1524" layer="91"/>
<label x="1821.18" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED121" gate="G$1" pin="5V"/>
<wire x1="1836.42" y1="12.7" x2="1836.42" y2="15.24" width="0.1524" layer="91"/>
<label x="1836.42" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED122" gate="G$1" pin="5V"/>
<wire x1="1851.66" y1="12.7" x2="1851.66" y2="15.24" width="0.1524" layer="91"/>
<label x="1851.66" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED123" gate="G$1" pin="5V"/>
<wire x1="1866.9" y1="12.7" x2="1866.9" y2="15.24" width="0.1524" layer="91"/>
<label x="1866.9" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED124" gate="G$1" pin="5V"/>
<wire x1="1882.14" y1="12.7" x2="1882.14" y2="15.24" width="0.1524" layer="91"/>
<label x="1882.14" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED125" gate="G$1" pin="5V"/>
<wire x1="1897.38" y1="12.7" x2="1897.38" y2="15.24" width="0.1524" layer="91"/>
<label x="1897.38" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED126" gate="G$1" pin="5V"/>
<wire x1="1912.62" y1="12.7" x2="1912.62" y2="15.24" width="0.1524" layer="91"/>
<label x="1912.62" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED127" gate="G$1" pin="5V"/>
<wire x1="1927.86" y1="12.7" x2="1927.86" y2="15.24" width="0.1524" layer="91"/>
<label x="1927.86" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="LED128" gate="G$1" pin="5V"/>
<wire x1="1943.1" y1="12.7" x2="1943.1" y2="15.24" width="0.1524" layer="91"/>
<label x="1943.1" y="15.24" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="LED1" gate="G$1" pin="GND"/>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="-5.08" width="0.1524" layer="91"/>
<label x="5.08" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED2" gate="G$1" pin="GND"/>
<wire x1="20.32" y1="-2.54" x2="20.32" y2="-5.08" width="0.1524" layer="91"/>
<label x="20.32" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="2"/>
<wire x1="0" y1="48.26" x2="7.62" y2="48.26" width="0.1524" layer="91"/>
<label x="7.62" y="48.26" size="1.778" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="LED3" gate="G$1" pin="GND"/>
<wire x1="35.56" y1="-2.54" x2="35.56" y2="-5.08" width="0.1524" layer="91"/>
<label x="35.56" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED4" gate="G$1" pin="GND"/>
<wire x1="50.8" y1="-2.54" x2="50.8" y2="-5.08" width="0.1524" layer="91"/>
<label x="50.8" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED5" gate="G$1" pin="GND"/>
<wire x1="66.04" y1="-2.54" x2="66.04" y2="-5.08" width="0.1524" layer="91"/>
<label x="66.04" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED6" gate="G$1" pin="GND"/>
<wire x1="81.28" y1="-2.54" x2="81.28" y2="-5.08" width="0.1524" layer="91"/>
<label x="81.28" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED7" gate="G$1" pin="GND"/>
<wire x1="96.52" y1="-2.54" x2="96.52" y2="-5.08" width="0.1524" layer="91"/>
<label x="96.52" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED8" gate="G$1" pin="GND"/>
<wire x1="111.76" y1="-2.54" x2="111.76" y2="-5.08" width="0.1524" layer="91"/>
<label x="111.76" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED9" gate="G$1" pin="GND"/>
<wire x1="127" y1="-2.54" x2="127" y2="-5.08" width="0.1524" layer="91"/>
<label x="127" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED10" gate="G$1" pin="GND"/>
<wire x1="142.24" y1="-2.54" x2="142.24" y2="-5.08" width="0.1524" layer="91"/>
<label x="142.24" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED11" gate="G$1" pin="GND"/>
<wire x1="157.48" y1="-2.54" x2="157.48" y2="-5.08" width="0.1524" layer="91"/>
<label x="157.48" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED12" gate="G$1" pin="GND"/>
<wire x1="172.72" y1="-2.54" x2="172.72" y2="-5.08" width="0.1524" layer="91"/>
<label x="172.72" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED13" gate="G$1" pin="GND"/>
<wire x1="187.96" y1="-2.54" x2="187.96" y2="-5.08" width="0.1524" layer="91"/>
<label x="187.96" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED14" gate="G$1" pin="GND"/>
<wire x1="203.2" y1="-2.54" x2="203.2" y2="-5.08" width="0.1524" layer="91"/>
<label x="203.2" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED15" gate="G$1" pin="GND"/>
<wire x1="218.44" y1="-2.54" x2="218.44" y2="-5.08" width="0.1524" layer="91"/>
<label x="218.44" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED16" gate="G$1" pin="GND"/>
<wire x1="233.68" y1="-2.54" x2="233.68" y2="-5.08" width="0.1524" layer="91"/>
<label x="233.68" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED17" gate="G$1" pin="GND"/>
<wire x1="248.92" y1="-2.54" x2="248.92" y2="-5.08" width="0.1524" layer="91"/>
<label x="248.92" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED18" gate="G$1" pin="GND"/>
<wire x1="264.16" y1="-2.54" x2="264.16" y2="-5.08" width="0.1524" layer="91"/>
<label x="264.16" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED19" gate="G$1" pin="GND"/>
<wire x1="279.4" y1="-2.54" x2="279.4" y2="-5.08" width="0.1524" layer="91"/>
<label x="279.4" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED20" gate="G$1" pin="GND"/>
<wire x1="294.64" y1="-2.54" x2="294.64" y2="-5.08" width="0.1524" layer="91"/>
<label x="294.64" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED21" gate="G$1" pin="GND"/>
<wire x1="309.88" y1="-2.54" x2="309.88" y2="-5.08" width="0.1524" layer="91"/>
<label x="309.88" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED22" gate="G$1" pin="GND"/>
<wire x1="325.12" y1="-2.54" x2="325.12" y2="-5.08" width="0.1524" layer="91"/>
<label x="325.12" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED23" gate="G$1" pin="GND"/>
<wire x1="340.36" y1="-2.54" x2="340.36" y2="-5.08" width="0.1524" layer="91"/>
<label x="340.36" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED24" gate="G$1" pin="GND"/>
<wire x1="355.6" y1="-2.54" x2="355.6" y2="-5.08" width="0.1524" layer="91"/>
<label x="355.6" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED25" gate="G$1" pin="GND"/>
<wire x1="370.84" y1="-2.54" x2="370.84" y2="-5.08" width="0.1524" layer="91"/>
<label x="370.84" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED26" gate="G$1" pin="GND"/>
<wire x1="386.08" y1="-2.54" x2="386.08" y2="-5.08" width="0.1524" layer="91"/>
<label x="386.08" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED27" gate="G$1" pin="GND"/>
<wire x1="401.32" y1="-2.54" x2="401.32" y2="-5.08" width="0.1524" layer="91"/>
<label x="401.32" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED28" gate="G$1" pin="GND"/>
<wire x1="416.56" y1="-2.54" x2="416.56" y2="-5.08" width="0.1524" layer="91"/>
<label x="416.56" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED29" gate="G$1" pin="GND"/>
<wire x1="431.8" y1="-2.54" x2="431.8" y2="-5.08" width="0.1524" layer="91"/>
<label x="431.8" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED30" gate="G$1" pin="GND"/>
<wire x1="447.04" y1="-2.54" x2="447.04" y2="-5.08" width="0.1524" layer="91"/>
<label x="447.04" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED31" gate="G$1" pin="GND"/>
<wire x1="462.28" y1="-2.54" x2="462.28" y2="-5.08" width="0.1524" layer="91"/>
<label x="462.28" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED32" gate="G$1" pin="GND"/>
<wire x1="477.52" y1="-2.54" x2="477.52" y2="-5.08" width="0.1524" layer="91"/>
<label x="477.52" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED33" gate="G$1" pin="GND"/>
<wire x1="492.76" y1="-2.54" x2="492.76" y2="-5.08" width="0.1524" layer="91"/>
<label x="492.76" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED34" gate="G$1" pin="GND"/>
<wire x1="508" y1="-2.54" x2="508" y2="-5.08" width="0.1524" layer="91"/>
<label x="508" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED35" gate="G$1" pin="GND"/>
<wire x1="523.24" y1="-2.54" x2="523.24" y2="-5.08" width="0.1524" layer="91"/>
<label x="523.24" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED36" gate="G$1" pin="GND"/>
<wire x1="538.48" y1="-2.54" x2="538.48" y2="-5.08" width="0.1524" layer="91"/>
<label x="538.48" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED37" gate="G$1" pin="GND"/>
<wire x1="553.72" y1="-2.54" x2="553.72" y2="-5.08" width="0.1524" layer="91"/>
<label x="553.72" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED38" gate="G$1" pin="GND"/>
<wire x1="568.96" y1="-2.54" x2="568.96" y2="-5.08" width="0.1524" layer="91"/>
<label x="568.96" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED39" gate="G$1" pin="GND"/>
<wire x1="584.2" y1="-2.54" x2="584.2" y2="-5.08" width="0.1524" layer="91"/>
<label x="584.2" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED40" gate="G$1" pin="GND"/>
<wire x1="599.44" y1="-2.54" x2="599.44" y2="-5.08" width="0.1524" layer="91"/>
<label x="599.44" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED41" gate="G$1" pin="GND"/>
<wire x1="614.68" y1="-2.54" x2="614.68" y2="-5.08" width="0.1524" layer="91"/>
<label x="614.68" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED42" gate="G$1" pin="GND"/>
<wire x1="629.92" y1="-2.54" x2="629.92" y2="-5.08" width="0.1524" layer="91"/>
<label x="629.92" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED43" gate="G$1" pin="GND"/>
<wire x1="645.16" y1="-2.54" x2="645.16" y2="-5.08" width="0.1524" layer="91"/>
<label x="645.16" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED44" gate="G$1" pin="GND"/>
<wire x1="660.4" y1="-2.54" x2="660.4" y2="-5.08" width="0.1524" layer="91"/>
<label x="660.4" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED45" gate="G$1" pin="GND"/>
<wire x1="675.64" y1="-2.54" x2="675.64" y2="-5.08" width="0.1524" layer="91"/>
<label x="675.64" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED46" gate="G$1" pin="GND"/>
<wire x1="690.88" y1="-2.54" x2="690.88" y2="-5.08" width="0.1524" layer="91"/>
<label x="690.88" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED47" gate="G$1" pin="GND"/>
<wire x1="706.12" y1="-2.54" x2="706.12" y2="-5.08" width="0.1524" layer="91"/>
<label x="706.12" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED48" gate="G$1" pin="GND"/>
<wire x1="721.36" y1="-2.54" x2="721.36" y2="-5.08" width="0.1524" layer="91"/>
<label x="721.36" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED49" gate="G$1" pin="GND"/>
<wire x1="736.6" y1="-2.54" x2="736.6" y2="-5.08" width="0.1524" layer="91"/>
<label x="736.6" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED50" gate="G$1" pin="GND"/>
<wire x1="751.84" y1="-2.54" x2="751.84" y2="-5.08" width="0.1524" layer="91"/>
<label x="751.84" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED51" gate="G$1" pin="GND"/>
<wire x1="767.08" y1="-2.54" x2="767.08" y2="-5.08" width="0.1524" layer="91"/>
<label x="767.08" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED52" gate="G$1" pin="GND"/>
<wire x1="782.32" y1="-2.54" x2="782.32" y2="-5.08" width="0.1524" layer="91"/>
<label x="782.32" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED53" gate="G$1" pin="GND"/>
<wire x1="797.56" y1="-2.54" x2="797.56" y2="-5.08" width="0.1524" layer="91"/>
<label x="797.56" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED54" gate="G$1" pin="GND"/>
<wire x1="812.8" y1="-2.54" x2="812.8" y2="-5.08" width="0.1524" layer="91"/>
<label x="812.8" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED55" gate="G$1" pin="GND"/>
<wire x1="828.04" y1="-2.54" x2="828.04" y2="-5.08" width="0.1524" layer="91"/>
<label x="828.04" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED56" gate="G$1" pin="GND"/>
<wire x1="843.28" y1="-2.54" x2="843.28" y2="-5.08" width="0.1524" layer="91"/>
<label x="843.28" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED57" gate="G$1" pin="GND"/>
<wire x1="858.52" y1="-2.54" x2="858.52" y2="-5.08" width="0.1524" layer="91"/>
<label x="858.52" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED58" gate="G$1" pin="GND"/>
<wire x1="873.76" y1="-2.54" x2="873.76" y2="-5.08" width="0.1524" layer="91"/>
<label x="873.76" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED59" gate="G$1" pin="GND"/>
<wire x1="889" y1="-2.54" x2="889" y2="-5.08" width="0.1524" layer="91"/>
<label x="889" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED60" gate="G$1" pin="GND"/>
<wire x1="904.24" y1="-2.54" x2="904.24" y2="-5.08" width="0.1524" layer="91"/>
<label x="904.24" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED61" gate="G$1" pin="GND"/>
<wire x1="919.48" y1="-2.54" x2="919.48" y2="-5.08" width="0.1524" layer="91"/>
<label x="919.48" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED62" gate="G$1" pin="GND"/>
<wire x1="934.72" y1="-2.54" x2="934.72" y2="-5.08" width="0.1524" layer="91"/>
<label x="934.72" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED63" gate="G$1" pin="GND"/>
<wire x1="949.96" y1="-2.54" x2="949.96" y2="-5.08" width="0.1524" layer="91"/>
<label x="949.96" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED64" gate="G$1" pin="GND"/>
<wire x1="965.2" y1="-2.54" x2="965.2" y2="-5.08" width="0.1524" layer="91"/>
<label x="965.2" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED65" gate="G$1" pin="GND"/>
<wire x1="980.44" y1="-2.54" x2="980.44" y2="-5.08" width="0.1524" layer="91"/>
<label x="980.44" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED66" gate="G$1" pin="GND"/>
<wire x1="995.68" y1="-2.54" x2="995.68" y2="-5.08" width="0.1524" layer="91"/>
<label x="995.68" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED67" gate="G$1" pin="GND"/>
<wire x1="1010.92" y1="-2.54" x2="1010.92" y2="-5.08" width="0.1524" layer="91"/>
<label x="1010.92" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED68" gate="G$1" pin="GND"/>
<wire x1="1026.16" y1="-2.54" x2="1026.16" y2="-5.08" width="0.1524" layer="91"/>
<label x="1026.16" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED69" gate="G$1" pin="GND"/>
<wire x1="1041.4" y1="-2.54" x2="1041.4" y2="-5.08" width="0.1524" layer="91"/>
<label x="1041.4" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED70" gate="G$1" pin="GND"/>
<wire x1="1056.64" y1="-2.54" x2="1056.64" y2="-5.08" width="0.1524" layer="91"/>
<label x="1056.64" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED71" gate="G$1" pin="GND"/>
<wire x1="1071.88" y1="-2.54" x2="1071.88" y2="-5.08" width="0.1524" layer="91"/>
<label x="1071.88" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED72" gate="G$1" pin="GND"/>
<wire x1="1087.12" y1="-2.54" x2="1087.12" y2="-5.08" width="0.1524" layer="91"/>
<label x="1087.12" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED73" gate="G$1" pin="GND"/>
<wire x1="1102.36" y1="-2.54" x2="1102.36" y2="-5.08" width="0.1524" layer="91"/>
<label x="1102.36" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED74" gate="G$1" pin="GND"/>
<wire x1="1117.6" y1="-2.54" x2="1117.6" y2="-5.08" width="0.1524" layer="91"/>
<label x="1117.6" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED75" gate="G$1" pin="GND"/>
<wire x1="1132.84" y1="-2.54" x2="1132.84" y2="-5.08" width="0.1524" layer="91"/>
<label x="1132.84" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED76" gate="G$1" pin="GND"/>
<wire x1="1148.08" y1="-2.54" x2="1148.08" y2="-5.08" width="0.1524" layer="91"/>
<label x="1148.08" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED77" gate="G$1" pin="GND"/>
<wire x1="1163.32" y1="-2.54" x2="1163.32" y2="-5.08" width="0.1524" layer="91"/>
<label x="1163.32" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED78" gate="G$1" pin="GND"/>
<wire x1="1178.56" y1="-2.54" x2="1178.56" y2="-5.08" width="0.1524" layer="91"/>
<label x="1178.56" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED79" gate="G$1" pin="GND"/>
<wire x1="1193.8" y1="-2.54" x2="1193.8" y2="-5.08" width="0.1524" layer="91"/>
<label x="1193.8" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED80" gate="G$1" pin="GND"/>
<wire x1="1209.04" y1="-2.54" x2="1209.04" y2="-5.08" width="0.1524" layer="91"/>
<label x="1209.04" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED81" gate="G$1" pin="GND"/>
<wire x1="1224.28" y1="-2.54" x2="1224.28" y2="-5.08" width="0.1524" layer="91"/>
<label x="1224.28" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED82" gate="G$1" pin="GND"/>
<wire x1="1239.52" y1="-2.54" x2="1239.52" y2="-5.08" width="0.1524" layer="91"/>
<label x="1239.52" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED83" gate="G$1" pin="GND"/>
<wire x1="1254.76" y1="-2.54" x2="1254.76" y2="-5.08" width="0.1524" layer="91"/>
<label x="1254.76" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED84" gate="G$1" pin="GND"/>
<wire x1="1270" y1="-2.54" x2="1270" y2="-5.08" width="0.1524" layer="91"/>
<label x="1270" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED85" gate="G$1" pin="GND"/>
<wire x1="1285.24" y1="-2.54" x2="1285.24" y2="-5.08" width="0.1524" layer="91"/>
<label x="1285.24" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED86" gate="G$1" pin="GND"/>
<wire x1="1300.48" y1="-2.54" x2="1300.48" y2="-5.08" width="0.1524" layer="91"/>
<label x="1300.48" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED87" gate="G$1" pin="GND"/>
<wire x1="1315.72" y1="-2.54" x2="1315.72" y2="-5.08" width="0.1524" layer="91"/>
<label x="1315.72" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED88" gate="G$1" pin="GND"/>
<wire x1="1330.96" y1="-2.54" x2="1330.96" y2="-5.08" width="0.1524" layer="91"/>
<label x="1330.96" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED89" gate="G$1" pin="GND"/>
<wire x1="1346.2" y1="-2.54" x2="1346.2" y2="-5.08" width="0.1524" layer="91"/>
<label x="1346.2" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED90" gate="G$1" pin="GND"/>
<wire x1="1361.44" y1="-2.54" x2="1361.44" y2="-5.08" width="0.1524" layer="91"/>
<label x="1361.44" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED91" gate="G$1" pin="GND"/>
<wire x1="1376.68" y1="-2.54" x2="1376.68" y2="-5.08" width="0.1524" layer="91"/>
<label x="1376.68" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED92" gate="G$1" pin="GND"/>
<wire x1="1391.92" y1="-2.54" x2="1391.92" y2="-5.08" width="0.1524" layer="91"/>
<label x="1391.92" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED93" gate="G$1" pin="GND"/>
<wire x1="1407.16" y1="-2.54" x2="1407.16" y2="-5.08" width="0.1524" layer="91"/>
<label x="1407.16" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED94" gate="G$1" pin="GND"/>
<wire x1="1422.4" y1="-2.54" x2="1422.4" y2="-5.08" width="0.1524" layer="91"/>
<label x="1422.4" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED95" gate="G$1" pin="GND"/>
<wire x1="1437.64" y1="-2.54" x2="1437.64" y2="-5.08" width="0.1524" layer="91"/>
<label x="1437.64" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED96" gate="G$1" pin="GND"/>
<wire x1="1452.88" y1="-2.54" x2="1452.88" y2="-5.08" width="0.1524" layer="91"/>
<label x="1452.88" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED97" gate="G$1" pin="GND"/>
<wire x1="1468.12" y1="-2.54" x2="1468.12" y2="-5.08" width="0.1524" layer="91"/>
<label x="1468.12" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED98" gate="G$1" pin="GND"/>
<wire x1="1483.36" y1="-2.54" x2="1483.36" y2="-5.08" width="0.1524" layer="91"/>
<label x="1483.36" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED99" gate="G$1" pin="GND"/>
<wire x1="1498.6" y1="-2.54" x2="1498.6" y2="-5.08" width="0.1524" layer="91"/>
<label x="1498.6" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED100" gate="G$1" pin="GND"/>
<wire x1="1513.84" y1="-2.54" x2="1513.84" y2="-5.08" width="0.1524" layer="91"/>
<label x="1513.84" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED101" gate="G$1" pin="GND"/>
<wire x1="1529.08" y1="-2.54" x2="1529.08" y2="-5.08" width="0.1524" layer="91"/>
<label x="1529.08" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED102" gate="G$1" pin="GND"/>
<wire x1="1544.32" y1="-2.54" x2="1544.32" y2="-5.08" width="0.1524" layer="91"/>
<label x="1544.32" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED103" gate="G$1" pin="GND"/>
<wire x1="1559.56" y1="-2.54" x2="1559.56" y2="-5.08" width="0.1524" layer="91"/>
<label x="1559.56" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED104" gate="G$1" pin="GND"/>
<wire x1="1574.8" y1="-2.54" x2="1574.8" y2="-5.08" width="0.1524" layer="91"/>
<label x="1574.8" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED105" gate="G$1" pin="GND"/>
<wire x1="1590.04" y1="-2.54" x2="1590.04" y2="-5.08" width="0.1524" layer="91"/>
<label x="1590.04" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED106" gate="G$1" pin="GND"/>
<wire x1="1605.28" y1="-2.54" x2="1605.28" y2="-5.08" width="0.1524" layer="91"/>
<label x="1605.28" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED107" gate="G$1" pin="GND"/>
<wire x1="1620.52" y1="-2.54" x2="1620.52" y2="-5.08" width="0.1524" layer="91"/>
<label x="1620.52" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED108" gate="G$1" pin="GND"/>
<wire x1="1635.76" y1="-2.54" x2="1635.76" y2="-5.08" width="0.1524" layer="91"/>
<label x="1635.76" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED109" gate="G$1" pin="GND"/>
<wire x1="1651" y1="-2.54" x2="1651" y2="-5.08" width="0.1524" layer="91"/>
<label x="1651" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED110" gate="G$1" pin="GND"/>
<wire x1="1666.24" y1="-2.54" x2="1666.24" y2="-5.08" width="0.1524" layer="91"/>
<label x="1666.24" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED111" gate="G$1" pin="GND"/>
<wire x1="1681.48" y1="-2.54" x2="1681.48" y2="-5.08" width="0.1524" layer="91"/>
<label x="1681.48" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED112" gate="G$1" pin="GND"/>
<wire x1="1696.72" y1="-2.54" x2="1696.72" y2="-5.08" width="0.1524" layer="91"/>
<label x="1696.72" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED113" gate="G$1" pin="GND"/>
<wire x1="1711.96" y1="-2.54" x2="1711.96" y2="-5.08" width="0.1524" layer="91"/>
<label x="1711.96" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED114" gate="G$1" pin="GND"/>
<wire x1="1727.2" y1="-2.54" x2="1727.2" y2="-5.08" width="0.1524" layer="91"/>
<label x="1727.2" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED115" gate="G$1" pin="GND"/>
<wire x1="1742.44" y1="-2.54" x2="1742.44" y2="-5.08" width="0.1524" layer="91"/>
<label x="1742.44" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED116" gate="G$1" pin="GND"/>
<wire x1="1757.68" y1="-2.54" x2="1757.68" y2="-5.08" width="0.1524" layer="91"/>
<label x="1757.68" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED117" gate="G$1" pin="GND"/>
<wire x1="1772.92" y1="-2.54" x2="1772.92" y2="-5.08" width="0.1524" layer="91"/>
<label x="1772.92" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED118" gate="G$1" pin="GND"/>
<wire x1="1788.16" y1="-2.54" x2="1788.16" y2="-5.08" width="0.1524" layer="91"/>
<label x="1788.16" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED119" gate="G$1" pin="GND"/>
<wire x1="1803.4" y1="-2.54" x2="1803.4" y2="-5.08" width="0.1524" layer="91"/>
<label x="1803.4" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED120" gate="G$1" pin="GND"/>
<wire x1="1818.64" y1="-2.54" x2="1818.64" y2="-5.08" width="0.1524" layer="91"/>
<label x="1818.64" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED121" gate="G$1" pin="GND"/>
<wire x1="1833.88" y1="-2.54" x2="1833.88" y2="-5.08" width="0.1524" layer="91"/>
<label x="1833.88" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED122" gate="G$1" pin="GND"/>
<wire x1="1849.12" y1="-2.54" x2="1849.12" y2="-5.08" width="0.1524" layer="91"/>
<label x="1849.12" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED123" gate="G$1" pin="GND"/>
<wire x1="1864.36" y1="-2.54" x2="1864.36" y2="-5.08" width="0.1524" layer="91"/>
<label x="1864.36" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED124" gate="G$1" pin="GND"/>
<wire x1="1879.6" y1="-2.54" x2="1879.6" y2="-5.08" width="0.1524" layer="91"/>
<label x="1879.6" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED125" gate="G$1" pin="GND"/>
<wire x1="1894.84" y1="-2.54" x2="1894.84" y2="-5.08" width="0.1524" layer="91"/>
<label x="1894.84" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED126" gate="G$1" pin="GND"/>
<wire x1="1910.08" y1="-2.54" x2="1910.08" y2="-5.08" width="0.1524" layer="91"/>
<label x="1910.08" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED127" gate="G$1" pin="GND"/>
<wire x1="1925.32" y1="-2.54" x2="1925.32" y2="-5.08" width="0.1524" layer="91"/>
<label x="1925.32" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
<segment>
<pinref part="LED128" gate="G$1" pin="GND"/>
<wire x1="1940.56" y1="-2.54" x2="1940.56" y2="-5.08" width="0.1524" layer="91"/>
<label x="1940.56" y="-5.08" size="1.778" layer="95" rot="R270" xref="yes"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="LED2" gate="G$1" pin="DIN"/>
<pinref part="LED1" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA" class="0">
<segment>
<pinref part="LED1" gate="G$1" pin="DIN"/>
<wire x1="-2.54" y1="5.08" x2="-5.08" y2="5.08" width="0.1524" layer="91"/>
<label x="-5.08" y="5.08" size="1.778" layer="95" rot="R180" xref="yes"/>
</segment>
<segment>
<pinref part="JP2" gate="A" pin="4"/>
<wire x1="40.64" y1="45.72" x2="45.72" y2="45.72" width="0.1524" layer="91"/>
<wire x1="45.72" y1="45.72" x2="45.72" y2="40.64" width="0.1524" layer="91"/>
<wire x1="45.72" y1="40.64" x2="48.26" y2="40.64" width="0.1524" layer="91"/>
<label x="48.26" y="40.64" size="1.778" layer="95" xref="yes"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="LED4" gate="G$1" pin="DIN"/>
<pinref part="LED3" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="LED3" gate="G$1" pin="DIN"/>
<pinref part="LED2" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="LED6" gate="G$1" pin="DIN"/>
<pinref part="LED5" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="LED8" gate="G$1" pin="DIN"/>
<pinref part="LED7" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="LED7" gate="G$1" pin="DIN"/>
<pinref part="LED6" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="LED5" gate="G$1" pin="DIN"/>
<pinref part="LED4" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="LED10" gate="G$1" pin="DIN"/>
<pinref part="LED9" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA1" class="0">
<segment>
<pinref part="LED9" gate="G$1" pin="DIN"/>
<pinref part="LED8" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="LED12" gate="G$1" pin="DIN"/>
<pinref part="LED11" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="LED11" gate="G$1" pin="DIN"/>
<pinref part="LED10" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="LED14" gate="G$1" pin="DIN"/>
<pinref part="LED13" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="LED16" gate="G$1" pin="DIN"/>
<pinref part="LED15" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="LED15" gate="G$1" pin="DIN"/>
<pinref part="LED14" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="LED13" gate="G$1" pin="DIN"/>
<pinref part="LED12" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="LED18" gate="G$1" pin="DIN"/>
<pinref part="LED17" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="LED20" gate="G$1" pin="DIN"/>
<pinref part="LED19" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="LED19" gate="G$1" pin="DIN"/>
<pinref part="LED18" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="LED22" gate="G$1" pin="DIN"/>
<pinref part="LED21" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<pinref part="LED24" gate="G$1" pin="DIN"/>
<pinref part="LED23" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<pinref part="LED23" gate="G$1" pin="DIN"/>
<pinref part="LED22" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$21" class="0">
<segment>
<pinref part="LED21" gate="G$1" pin="DIN"/>
<pinref part="LED20" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$22" class="0">
<segment>
<pinref part="LED26" gate="G$1" pin="DIN"/>
<pinref part="LED25" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA2" class="0">
<segment>
<pinref part="LED25" gate="G$1" pin="DIN"/>
<pinref part="LED24" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$23" class="0">
<segment>
<pinref part="LED28" gate="G$1" pin="DIN"/>
<pinref part="LED27" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$24" class="0">
<segment>
<pinref part="LED27" gate="G$1" pin="DIN"/>
<pinref part="LED26" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$25" class="0">
<segment>
<pinref part="LED30" gate="G$1" pin="DIN"/>
<pinref part="LED29" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$26" class="0">
<segment>
<pinref part="LED32" gate="G$1" pin="DIN"/>
<pinref part="LED31" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$27" class="0">
<segment>
<pinref part="LED31" gate="G$1" pin="DIN"/>
<pinref part="LED30" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$28" class="0">
<segment>
<pinref part="LED29" gate="G$1" pin="DIN"/>
<pinref part="LED28" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$29" class="0">
<segment>
<pinref part="LED17" gate="G$1" pin="DIN"/>
<pinref part="LED16" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$30" class="0">
<segment>
<pinref part="LED34" gate="G$1" pin="DIN"/>
<pinref part="LED33" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$31" class="0">
<segment>
<pinref part="LED36" gate="G$1" pin="DIN"/>
<pinref part="LED35" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$32" class="0">
<segment>
<pinref part="LED35" gate="G$1" pin="DIN"/>
<pinref part="LED34" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$33" class="0">
<segment>
<pinref part="LED38" gate="G$1" pin="DIN"/>
<pinref part="LED37" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$34" class="0">
<segment>
<pinref part="LED40" gate="G$1" pin="DIN"/>
<pinref part="LED39" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$35" class="0">
<segment>
<pinref part="LED39" gate="G$1" pin="DIN"/>
<pinref part="LED38" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$36" class="0">
<segment>
<pinref part="LED37" gate="G$1" pin="DIN"/>
<pinref part="LED36" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$37" class="0">
<segment>
<pinref part="LED42" gate="G$1" pin="DIN"/>
<pinref part="LED41" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA3" class="0">
<segment>
<pinref part="LED41" gate="G$1" pin="DIN"/>
<pinref part="LED40" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$38" class="0">
<segment>
<pinref part="LED44" gate="G$1" pin="DIN"/>
<pinref part="LED43" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$39" class="0">
<segment>
<pinref part="LED43" gate="G$1" pin="DIN"/>
<pinref part="LED42" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$40" class="0">
<segment>
<pinref part="LED46" gate="G$1" pin="DIN"/>
<pinref part="LED45" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$41" class="0">
<segment>
<pinref part="LED48" gate="G$1" pin="DIN"/>
<pinref part="LED47" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$42" class="0">
<segment>
<pinref part="LED47" gate="G$1" pin="DIN"/>
<pinref part="LED46" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$43" class="0">
<segment>
<pinref part="LED45" gate="G$1" pin="DIN"/>
<pinref part="LED44" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$44" class="0">
<segment>
<pinref part="LED50" gate="G$1" pin="DIN"/>
<pinref part="LED49" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$45" class="0">
<segment>
<pinref part="LED52" gate="G$1" pin="DIN"/>
<pinref part="LED51" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$46" class="0">
<segment>
<pinref part="LED51" gate="G$1" pin="DIN"/>
<pinref part="LED50" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$47" class="0">
<segment>
<pinref part="LED54" gate="G$1" pin="DIN"/>
<pinref part="LED53" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$48" class="0">
<segment>
<pinref part="LED56" gate="G$1" pin="DIN"/>
<pinref part="LED55" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$49" class="0">
<segment>
<pinref part="LED55" gate="G$1" pin="DIN"/>
<pinref part="LED54" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$50" class="0">
<segment>
<pinref part="LED53" gate="G$1" pin="DIN"/>
<pinref part="LED52" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$51" class="0">
<segment>
<pinref part="LED58" gate="G$1" pin="DIN"/>
<pinref part="LED57" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA4" class="0">
<segment>
<pinref part="LED57" gate="G$1" pin="DIN"/>
<pinref part="LED56" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$52" class="0">
<segment>
<pinref part="LED60" gate="G$1" pin="DIN"/>
<pinref part="LED59" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$53" class="0">
<segment>
<pinref part="LED59" gate="G$1" pin="DIN"/>
<pinref part="LED58" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$54" class="0">
<segment>
<pinref part="LED62" gate="G$1" pin="DIN"/>
<pinref part="LED61" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$55" class="0">
<segment>
<pinref part="LED64" gate="G$1" pin="DIN"/>
<pinref part="LED63" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$56" class="0">
<segment>
<pinref part="LED63" gate="G$1" pin="DIN"/>
<pinref part="LED62" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$57" class="0">
<segment>
<pinref part="LED61" gate="G$1" pin="DIN"/>
<pinref part="LED60" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$58" class="0">
<segment>
<pinref part="LED49" gate="G$1" pin="DIN"/>
<pinref part="LED48" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$59" class="0">
<segment>
<pinref part="LED33" gate="G$1" pin="DIN"/>
<pinref part="LED32" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$60" class="0">
<segment>
<pinref part="LED66" gate="G$1" pin="DIN"/>
<pinref part="LED65" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA5" class="0">
<segment>
<pinref part="LED65" gate="G$1" pin="DIN"/>
<pinref part="LED64" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$61" class="0">
<segment>
<pinref part="LED68" gate="G$1" pin="DIN"/>
<pinref part="LED67" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$62" class="0">
<segment>
<pinref part="LED67" gate="G$1" pin="DIN"/>
<pinref part="LED66" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$63" class="0">
<segment>
<pinref part="LED70" gate="G$1" pin="DIN"/>
<pinref part="LED69" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$64" class="0">
<segment>
<pinref part="LED72" gate="G$1" pin="DIN"/>
<pinref part="LED71" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$65" class="0">
<segment>
<pinref part="LED71" gate="G$1" pin="DIN"/>
<pinref part="LED70" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$66" class="0">
<segment>
<pinref part="LED69" gate="G$1" pin="DIN"/>
<pinref part="LED68" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$67" class="0">
<segment>
<pinref part="LED74" gate="G$1" pin="DIN"/>
<pinref part="LED73" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA6" class="0">
<segment>
<pinref part="LED73" gate="G$1" pin="DIN"/>
<pinref part="LED72" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$68" class="0">
<segment>
<pinref part="LED76" gate="G$1" pin="DIN"/>
<pinref part="LED75" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$69" class="0">
<segment>
<pinref part="LED75" gate="G$1" pin="DIN"/>
<pinref part="LED74" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$70" class="0">
<segment>
<pinref part="LED78" gate="G$1" pin="DIN"/>
<pinref part="LED77" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$71" class="0">
<segment>
<pinref part="LED80" gate="G$1" pin="DIN"/>
<pinref part="LED79" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$72" class="0">
<segment>
<pinref part="LED79" gate="G$1" pin="DIN"/>
<pinref part="LED78" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$73" class="0">
<segment>
<pinref part="LED77" gate="G$1" pin="DIN"/>
<pinref part="LED76" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$74" class="0">
<segment>
<pinref part="LED82" gate="G$1" pin="DIN"/>
<pinref part="LED81" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$75" class="0">
<segment>
<pinref part="LED84" gate="G$1" pin="DIN"/>
<pinref part="LED83" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$76" class="0">
<segment>
<pinref part="LED83" gate="G$1" pin="DIN"/>
<pinref part="LED82" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$77" class="0">
<segment>
<pinref part="LED86" gate="G$1" pin="DIN"/>
<pinref part="LED85" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$78" class="0">
<segment>
<pinref part="LED88" gate="G$1" pin="DIN"/>
<pinref part="LED87" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$79" class="0">
<segment>
<pinref part="LED87" gate="G$1" pin="DIN"/>
<pinref part="LED86" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$80" class="0">
<segment>
<pinref part="LED85" gate="G$1" pin="DIN"/>
<pinref part="LED84" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$81" class="0">
<segment>
<pinref part="LED90" gate="G$1" pin="DIN"/>
<pinref part="LED89" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA7" class="0">
<segment>
<pinref part="LED89" gate="G$1" pin="DIN"/>
<pinref part="LED88" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$82" class="0">
<segment>
<pinref part="LED92" gate="G$1" pin="DIN"/>
<pinref part="LED91" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$83" class="0">
<segment>
<pinref part="LED91" gate="G$1" pin="DIN"/>
<pinref part="LED90" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$84" class="0">
<segment>
<pinref part="LED94" gate="G$1" pin="DIN"/>
<pinref part="LED93" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$85" class="0">
<segment>
<pinref part="LED96" gate="G$1" pin="DIN"/>
<pinref part="LED95" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$86" class="0">
<segment>
<pinref part="LED95" gate="G$1" pin="DIN"/>
<pinref part="LED94" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$87" class="0">
<segment>
<pinref part="LED93" gate="G$1" pin="DIN"/>
<pinref part="LED92" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$88" class="0">
<segment>
<pinref part="LED81" gate="G$1" pin="DIN"/>
<pinref part="LED80" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$89" class="0">
<segment>
<pinref part="LED98" gate="G$1" pin="DIN"/>
<pinref part="LED97" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$90" class="0">
<segment>
<pinref part="LED100" gate="G$1" pin="DIN"/>
<pinref part="LED99" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$91" class="0">
<segment>
<pinref part="LED99" gate="G$1" pin="DIN"/>
<pinref part="LED98" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$92" class="0">
<segment>
<pinref part="LED102" gate="G$1" pin="DIN"/>
<pinref part="LED101" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$93" class="0">
<segment>
<pinref part="LED104" gate="G$1" pin="DIN"/>
<pinref part="LED103" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$94" class="0">
<segment>
<pinref part="LED103" gate="G$1" pin="DIN"/>
<pinref part="LED102" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$95" class="0">
<segment>
<pinref part="LED101" gate="G$1" pin="DIN"/>
<pinref part="LED100" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$96" class="0">
<segment>
<pinref part="LED106" gate="G$1" pin="DIN"/>
<pinref part="LED105" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA8" class="0">
<segment>
<pinref part="LED105" gate="G$1" pin="DIN"/>
<pinref part="LED104" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$97" class="0">
<segment>
<pinref part="LED108" gate="G$1" pin="DIN"/>
<pinref part="LED107" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$98" class="0">
<segment>
<pinref part="LED107" gate="G$1" pin="DIN"/>
<pinref part="LED106" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$99" class="0">
<segment>
<pinref part="LED110" gate="G$1" pin="DIN"/>
<pinref part="LED109" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$100" class="0">
<segment>
<pinref part="LED112" gate="G$1" pin="DIN"/>
<pinref part="LED111" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$101" class="0">
<segment>
<pinref part="LED111" gate="G$1" pin="DIN"/>
<pinref part="LED110" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$102" class="0">
<segment>
<pinref part="LED109" gate="G$1" pin="DIN"/>
<pinref part="LED108" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$103" class="0">
<segment>
<pinref part="LED114" gate="G$1" pin="DIN"/>
<pinref part="LED113" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$104" class="0">
<segment>
<pinref part="LED116" gate="G$1" pin="DIN"/>
<pinref part="LED115" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$105" class="0">
<segment>
<pinref part="LED115" gate="G$1" pin="DIN"/>
<pinref part="LED114" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$106" class="0">
<segment>
<pinref part="LED118" gate="G$1" pin="DIN"/>
<pinref part="LED117" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$107" class="0">
<segment>
<pinref part="LED120" gate="G$1" pin="DIN"/>
<pinref part="LED119" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$108" class="0">
<segment>
<pinref part="LED119" gate="G$1" pin="DIN"/>
<pinref part="LED118" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$109" class="0">
<segment>
<pinref part="LED117" gate="G$1" pin="DIN"/>
<pinref part="LED116" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$110" class="0">
<segment>
<pinref part="LED122" gate="G$1" pin="DIN"/>
<pinref part="LED121" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="DATA9" class="0">
<segment>
<pinref part="LED121" gate="G$1" pin="DIN"/>
<pinref part="LED120" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$111" class="0">
<segment>
<pinref part="LED124" gate="G$1" pin="DIN"/>
<pinref part="LED123" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$112" class="0">
<segment>
<pinref part="LED123" gate="G$1" pin="DIN"/>
<pinref part="LED122" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$113" class="0">
<segment>
<pinref part="LED126" gate="G$1" pin="DIN"/>
<pinref part="LED125" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$114" class="0">
<segment>
<pinref part="LED128" gate="G$1" pin="DIN"/>
<pinref part="LED127" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$115" class="0">
<segment>
<pinref part="LED127" gate="G$1" pin="DIN"/>
<pinref part="LED126" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$116" class="0">
<segment>
<pinref part="LED125" gate="G$1" pin="DIN"/>
<pinref part="LED124" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$117" class="0">
<segment>
<pinref part="LED113" gate="G$1" pin="DIN"/>
<pinref part="LED112" gate="G$1" pin="DOUT"/>
</segment>
</net>
<net name="N$118" class="0">
<segment>
<pinref part="LED97" gate="G$1" pin="DIN"/>
<pinref part="LED96" gate="G$1" pin="DOUT"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
