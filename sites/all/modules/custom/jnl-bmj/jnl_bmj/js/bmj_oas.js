<!--
//configuration
OAS_url = 'http://oas.services.bmj.com/RealMedia/ads/';
OAS_sitepage = window.location.hostname + window.location.pathname;
OAS_listpos = 'Middle1,Middle2,Top';
OAS_query = '';
OAS_target = '_top';
//end of configuration
OAS_version = 10;
OAS_rn = '001234567890'; OAS_rns = '1234567890';
OAS_rn = new String (Math.random()); OAS_rns = OAS_rn.substring (2, 11);
function OAS_NORMAL(pos) {
document.write('<A HREF="' + OAS_url + 'click_nx.ads/' + OAS_sitepage + '/1' + OAS_rns + '@' + OAS_listpos + '!' + pos + '?' + OAS_query + '" TARGET=' + OAS_target + '>');
document.write('<IMG SRC="' + OAS_url + 'adstream_nx.ads/' + OAS_sitepage + '/1' + OAS_rns + '@' + OAS_listpos + '!' + pos + '?' + OAS_query + '" BORDER=0></A>');
}
 //-->
<!--  
 OAS_version = 11;  
if ((navigator.userAgent.indexOf('Mozilla/3') != -1) || (navigator.userAgent.indexOf('Mozilla/4.0 WebTV') != -1))  
 OAS_version = 10;
if (OAS_version >= 11)
document.write('<SCR' + 'IPT LANGUAGE=JavaScript1.1 SRC="' + OAS_url + 'adstream_mjx.ads/' + OAS_sitepage + '/1' + OAS_rns + '@' + OAS_listpos + '?' + OAS_query + '"><\/SCRIPT>');//-->

<!--  
 document.write('');  
function OAS_AD(pos) {  
if (OAS_version >= 11 && typeof(OAS_RICH)!='undefined')
 OAS_RICH(pos);  
else
 OAS_NORMAL(pos);  
}  //-->
 