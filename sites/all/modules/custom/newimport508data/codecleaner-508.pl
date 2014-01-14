#!/usr/bin/perl 
#
# Purpose: 	Used to clean the unwanted tags/attributes added when excel file was "saved as"
#		html file.
#
# Usage: codecleaner-508.pl                  OldFileName NewFileName
#        codecleaner-508.pl -d(Style/Option) OldFileName NewFileName
#        codecleaner-508.pl -help
#        codecleaner-508.pl -version 
# Option: 
#	-d with out inserting SSI header/footer 
# History: 
# 	06/20/2011:	By Jinhui (Ver. 5.0)
#			Modified for running in drupal. Add RunMode parameter and set it to "silent".
#	05/16/2011:	By Jinhui
#			Add additional function to process title lines before the table and keys/notes/sources after the table.
# 	05/03/2011:	By Jinhui
#			Redesigned to be using Color style and 508 data table processing (in Drupal).
#	02/30/2004: 	By Jinhui
# 			fix bug in <td ....>
# 	01/16/2004:	By Jinhui Huang, (original)
#
# Copyrighted, exclusively for use at RITA/DOT only.
#
my $VERSION = 5.0;
############################ User Defined Parameters ###############################
$RunMode = "silent";	# options: silent, debug.
#$RunMode = ""; 	# default: debug mode

####################### function list ##############################################
# main()
# printInternalFile()
####################################################################################
#======================= main() ========================
# print out version number if asked.
my $UsageMsg = "\tUsage: $0          OldFileName NewFileName       
	       $0 -d       OldFileName NewFileName
	       $0 -help
	       $0 -version\n\n";

if(@ARGV == 1) {
	if($ARGV[0] =~ m/-help/i)    { die "\n$UsageMsg";             };	# for option -help
	if($ARGV[0] =~ m/-version/i) { die "\nVersion: $VERSION\n\n"; };	# for option -version
}
if((@ARGV < 2) || (@ARGV > 3)) {
  die "\n$UsageMsg";
}

if($RunMode ne "silent") {	# print if not in silent mode
   print "\n-----------------------------------\n";
   print "Program  codecleaner-508.pl  start:  \n";
   print "-----------------------------------  \n";
}

if(@ARGV == 2) {
  $StyleOption = "";
  $OldFN = $ARGV[0];
  $NewFN = $ARGV[1];
}
else  {   			# if (@ARGV == 3)
  $StyleOption = $ARGV[0];
  $StyleOption =~ s/-//g; 	# remove "-" in "-d"
  $OldFN       = $ARGV[1];
  $NewFN       = $ARGV[2];
}

if($RunMode ne "silent") {	# print if not in silent mode
   print "Style/Option : [$StyleOption]\n";
   print "Original file: [$OldFN]\n";
   print "New  filename: [$NewFN]\n\n";
}
#------------- open input/output files ---------------------------
open(OFN,   $OldFN ) || die "Can't open $OldFN: $!\n";
open(NFN, ">$NewFN") || die "Can't open $NewFN: $!\n";

#------------- global variable -----------------------------------
# table cell classes for left/right alignment
my $NumberClass = "cellright";     	# "RightAlign";
my $StringClass = "cellleft";		# "LeftAlign";

# <!--(table508parameters):MaxColHeadLevel,MaxRowHeadLevel,MaxRowHeadLayer:[1,1,2]-->
my $MaxColHeadLevel = 0;	# default value: not set yet.
my $MaxRowHeadLevel = 0;
my $MaxRowHeadLayer = 0;

# used to convert style/color info to class name
my %SColorEClass=(		# match Style color to 508 Encoding class
	'558822'=>'RHlevel1',
	'55AA22'=>'RHlevel2',
	'55DD33'=>'RHlevel3',
	'55EE99'=>'RHlevel4',
	
	'CC4433'=>'CHlevel1',
	'CC6655'=>'CHlevel2',
	'CC8866'=>'CHlevel3',
	'CCAA99'=>'CHlevel4',
	
	'336699'=>'RHlayer1',
	'3377CC'=>'RHlayer2',
	'33AACC'=>'RHlayer3',
	'33CCCC'=>'RHlayer4',
	'33DDEE'=>'RHlayer5',
	'33FFFF'=>'RHlayer6',
);

#while( ($color, $class) = each %SColorEClass )  {
#	print "$color -> $class => $SColorEClass{$color}\n";
#}

# used to find beginning and ending of the data table
my $TableStartColor = 'EE0ACC'; 	# name of the background color
my $TableStopColor  = 'EEAACC';
my $TableStartClass = '';
my $TableStopClass  = '';

#my %SClassEClass={};		# match Style Class to 508 Encoding Class, like "xl784291" -> "CHlevel3"
#my %SClassBold={};		# match Style font-class/td-class with the "bold", like
				#       "font86628" -> "bold" (font-weight:700;)
				#       "xl916628"  -> "bold" (font-weight:700;)
#my %SClassItalic={};		# match Style font-class/td-class with the "italic", like 
				#       "font86628" -> "italic" (font-style:italic;)
				#       "xl856628"  -> "italic" (font-style:italic;)

#my @AllLines = <OFN>; 		# read in all lines from old file
#------------------------------------------------------------------
my $KeepFsValue = undef $/;	# keep undef value for later re-define; each read is a whole file
my $AllInOneLine = <OFN>; 	# read in all lines from old file

#print "$AllInOneLine\n";

# find and save head style 
if($AllInOneLine =~ /<head>.*?(<style .*?<\/style>)\s<\/head>/gsi  ) {
	my $classset     = $1;				# whole <style> in <head>
	#my $classfontset = $1;				# for bold, italic fonts/styles processing
	
	#print "classes catched:\n$classset\n";
	$classset =~ s/<style .*?(\.\w+\s.*)\s+-->\s<\/style>/$1/gsi;
	#print "classes catched:\n$classset\n";		# now, it include classes (.fontxxx & .xlxxx) only
	#$classfontset = $classset;			# for bold, italic fonts/styles processing
	
	# find "background:#" as in "background:#558822;" and build %SClassEClass.
	# in the meantime, find TableStartColor/TableStopColor, and set $TableStartClass & $TableStopClass
	&BuildMatchTable($classset, "SClassEClass");
	
	# find bold (font-weight:700) and build %SClassBold
	&BuildMatchTable($classset, "SClassBold");
	
	# find italic (font-style:italic) and build %SClassItalic
	&BuildMatchTable($classset, "SClassItalic");
	
	# following prints are for testing only
	if($RunMode ne "silent") {			# print if not in silent mode
	  while( ($sclass, $eclass) = each %SClassEClass )  {
		print "=== $sclass -> $eclass,  $SClassEClass{$sclass}\n";	
	  }						# output like, "=== xl10412253 -> CHlevel1,  CHlevel1"	
	  while( ($sclass, $eclass) = each %SClassBold )  {
		print "+++ $sclass -> $eclass\n";	
	  }	
	  while( ($sclass, $eclass) = each %SClassItalic )  {
		print "--- $sclass -> $eclass\n";	
	  }
	}
}

if($RunMode ne "silent") {	# print if not in silent mode
   print "###############################\n";
}
#die "end of match table build section...\n";


###############################################################
# process whole text (pre table + table + post table texts) to 
# handle bold, italic fonts/styles.

#$AllInOneLine =~ s/(<table .*?class=$TableStartClass.*?<\/tr>)/<table>/gsi;
#if( $AllInOneLine =~ /(<td .*? class=xl846628.*?>)(.*?)(<)/si) {    die "xl846628: $2\n"; }

# process bold first:
# the following block is too slow ... rewrite it???????????????
while( ($sclass, $tmpbold) = each %SClassBold )  {	
     #if( $AllInOneLine =~ /(<td .*? class=$sclass.*?>)(.*?)(<\/td)/si) { print "$sclass: $2\n"; }
     if( $sclass =~ /font/i) {	# for class of "font######"
        $AllInOneLine =~ s/(<font .*?class=\"?$sclass\"?.*?>)(.*?)(<\/font)/$1<b>$2<\/b>$3/gsi;
     }
     else  {			# for class of "xl########"
        $AllInOneLine =~ s/(<td .*?class=$sclass.*?>)(.*?)(<\/td)/$1<b>$2<\/b>$3/gsi;
     }
}

if($RunMode ne "silent") {  print "end of processing bold\n";  } 	# print if not in silent mode


# then, process itlic:
while( ($sclass, $tmpitalic) = each %SClassItalic )  {	
     if( $sclass =~ /font/i) {	# for class of "font######"
        #print " * AllInOneLine *: $AllInOneLine\n\n";
        #print "check this fontclass: $sclass --- \n";
        #if( $AllInOneLine =~ /(<font.*?class=\"?$sclass\"?.*?>)(.*?)(<\/font)(.*$)/si) { print "=== $sclass: $2\n"; }
        #$remainpart = $4;
        #while( $remainpart =~ /(<font.*?class=\"?$sclass\"?.*?>)(.*?)(<\/font)(.*$)/si ) {
        #   $remainpart = $4;
        #   print "=== $sclass: $2\n"; 
        #}
        
        $AllInOneLine =~ s/(<font.*?class=\"?$sclass\"?.*?>)(.*?)(<\/font)/$1<i>$2<\/i>$3/gsi;
     }
     else  {			# for class of "xl########"
        $AllInOneLine =~ s/(<td.*?class=$sclass.*?>)(.*?)(<\/td)/$1<i>$2<\/i>$3/gsi;
     }
}

#die "AllInOneLine33: AllInOneLine\n\n";

###############################################################
# process Pre table text

$AllInOneLine =~ s/(<table .*?class=$TableStartClass.*?<\/tr>)/<table>/gsi;
#$AllInOneLine =~ s/(<table .*?$TableStartClass.*?<\/tr>)/<table>/gsi;
my $TmpPreTableText = $1;
my $PreTableText = "";

$IsTableTitle = 1;		# 1st line, 1st td is the table title
#while(length($TmpPreTableText) > 20) {
while( $TmpPreTableText =~ /<td /i ) {
   $TmpPreTableText =~ s/<td .*?>(.*?)<\/td>//si;
   my $tmptd = $1;
   $tmptd    = &CleanText($tmptd);
   if( (length($tmptd) == 0) || ($tmptd =~ /\s*&nbsp;\s*/i ) ) {  next;  }		# do nothing for the empty <td>
   if($IsTableTitle) { $tmptd = "<h3>".$tmptd."</h3>\n"; }
   else              { $tmptd = "<p>".$tmptd."</p>\n"; } 
   $PreTableText = $PreTableText.$tmptd;
   
   $IsTableTitle = 0;		# only the 1st row/td is title line
}
#print "----  $PreTableText\n";
#$PreTableText = &CleanText($PreTableText);
#$PreTableText = "<h3>".$PreTableText."</h3>";

if($RunMode ne "silent") {  print "====  $PreTableText\n";  } 	# print if not in silent mode 

#die " stop at 23344\n\n";

$AllInOneLine =~ s/(<table>)/$PreTableText\n$1/si;
###############################################################
# process Data table and Post table text

# Find and modify data table and post table text
#$AllInOneLine =~ s/(<table .*?class=$TableStartClass.*?<\/tr>)/<table>/gsi;
#$AllInOneLine =~  s/(<tr .*>\s*)(<tr [^>]*?>\s*<td .*?class=$TableStopClass.*?<\/tr>)(.*?)(<\/table>)/$1$4/gsi;
my $PostTableText = "";
my $DataTableRows = "";
if($AllInOneLine =~ /(<tr .*>\s*)(<tr [^>]*?>\s*<td .*?class=$TableStopClass.*?<\/tr>)(.*?)(<\/table>)/gsi) {   
   # $1: table <tr> line; $2: table end marker <tr> line; $3: Post table text; $4: </table>
   #print "#1: $1\n#2: $2\n#3: $3\n#4: $4\n\n"; 
   my $TmpDataTableRows = $1;		# all rows
   my $TmpPostTableText = $3;
   
   #########################
   # Build $DataTableRows ( to remove COMMON empty column)
   #print "TmpDataTableRows:\n$TmpDataTableRows\n";
   $DataTableRows = &RemoveEmptyColumn($TmpDataTableRows);   
   #print "DataTableRows:\n$DataTableRows\n";
   
   #########################
   # Build $PostTableText
   my $PreviousClass = "note";		# default 
   while( $TmpPostTableText =~ /<td /i ) {
      $TmpPostTableText =~ s/<td .*?>(.*?)<\/td>//si;
      #$PostTableText = $PostTableText.$1."<br />\n";
      my $TdContent = $1;
      my $TmpClass = "";
      if(length($TdContent) != 0) { 
        
        if( ($TdContent =~ /^\s*<sup>/i) || ($TdContent =~ /^\s*\*/) || ($TdContent =~ /^\s*NOTES?/i) ) {
           $TmpClass = "note";
        }
        elsif($TdContent =~ /^\s*SOURCES?/i) {
           $TmpClass = "source";
        }
        elsif($TdContent =~ /^\s*KEY/i) {
           $TmpClass = "key";
        }
        else {
           $TmpClass = $PreviousClass;
        }
        $PostTableText = $PostTableText."<p class=\"$TmpClass\">".$TdContent."</p>\n\n"; 
        $PreviousClass = $TmpClass;
      }
   }
   
   #print ">>>>  $PostTableText\n";
   $PostTableText = &CleanText($PostTableText);
   #print ">>>>\n  $PostTableText\n";
   
   #die "found 1111\n$2\n-----\n$3\n1111"; 
   #die "stop @ 5678\n";
}
# modify post table text
#$AllInOneLine =~ s/(<tr .*>\s*)(<tr [^>]*?>\s*<td .*?class=$TableStopClass.*?<\/tr>)(.*?)(<\/table>)/$1$4<br \/><br \/>\n\n$PostTableText/gsi;
$AllInOneLine =~ s/(<tr .*>\s*)(<tr [^>]*?>\s*<td .*?class=$TableStopClass.*?<\/tr>)(.*?)(<\/table>)/$DataTableRows$4<br \/><br \/>\n\n$PostTableText/gsi;

#die "\n\n\n$AllInOneLine\n";
#die "TmpPreTableText: $TmpPreTableText\n";
#die "stop 3434\n";
###############################################################
# remove head style (no need to this step)
# $AllInOneLine =~ s/(<head>.*?)(<style .*?<\/style>)(\s<\/head>)/$1$3/gsi;

if($RunMode ne "silent") {  print "$AllInOneLine\n"; }		# print if not in silent mode

#die "test point XXXX\n";

# ====== remove some tags (keep this order!): ======

# remove <meta *>
$AllInOneLine =~ s/<meta .*?>//gsi;		

# remove <link rel*>
$AllInOneLine =~ s/<link rel.*?>//gsi;		

# remove <style>*</style> and  style=* attribute
$AllInOneLine =~ s/<style>.*?<\/style>//gsi;	
$AllInOneLine =~ s/style=\S*?>/>/gsi;	# for style which is the last attribute
$AllInOneLine =~ s/style=\S*? / /gsi;	# for style which not the last attribute

# remove <xml*>*</xml>
$AllInOneLine =~ s/<xml.*?>.*?<\/xml>//gsi;	

# remove <![if *]>*<![endif]> and <!--[if *]>*<![endif]-->
#$AllInOneLine =~ s/<!\[if .*?endif\]>//gs;			# keep for ref.
#$AllInOneLine =~ s/<!\[if !.+?\]>.*?<!\[endif\]>//gs;		# keep for ref.
$AllInOneLine =~ s/<!(--)?\[if .+?\]>.*?<!\[endif\](--)?>//gsi;	

# remove comments
$AllInOneLine =~ s/<!--.*?-->//gs;		

# remove <col *>
$AllInOneLine =~ s/<col .*?>//gsi;

# remove <font*>, </font>
$AllInOneLine =~ s/<font.*?>//gsi;
$AllInOneLine =~ s/<\/font>//gsi;

# remove <span*>, </span>
$AllInOneLine =~ s/<span.*?>//gsi;
$AllInOneLine =~ s/<\/span>//gsi;

# remove <div*>, </div>
$AllInOneLine =~ s/<div.*?>//gsi;
$AllInOneLine =~ s/<\/div>//gsi;

# remove empty lines:
#$AllInOneLine =~ s/\n\n+/\n/g;					# keep for ref.
$AllInOneLine =~ s/\n( *\n)+/\n/g;	



# ====== modify some tags (keep this order!): ======

# modify <html *> to <html>
$AllInOneLine =~ s/<html.+?>/<html>/gsi;

# modify <head *> to <head> then insert default <title>, <meta>, and css line
my $head_section_string = "<head>\n\t<title>Bureau of Transportation Statistics (BTS)<\/title>
	<meta http-equiv=\"Content-Type\" content=\"text\/html; charset=ISO-8859-1\" \/>
	<link rel=\"stylesheet\" type=\"text\/css\" href=\"/css\/bts.css\" />  \n<\/head>";
$AllInOneLine =~ s/<head.*?>.*?<\/head>/$head_section_string/gsi;

# modify <body *> to <body> then, by default, adding ssi header/footer. 
# 	Please note, the header/footer is optional. They can be avoided by using 
#	command line option -d.  
my $header_string = "\n\t<a class=\"skip\" href=\"#Skip_to_content\" title=\"Skip to content\">Skip to content<\/a><br>
	<!--Do not delete or alter the following SSI tag(s)-->
	<!--#include virtual=\"\/template\/globalheader_010605.txt\" -->
	<a id=\"Skip_to_content\" name=\"Skip_to_content\"><\/a>";
my $footer_string = "\n\t<!--Do not delete or alter the following SSI tag(s)-->
	<!--#include virtual=\"\/template\/globalfooter_010605.txt\" -->\n";
if ($StyleOption =~ /^D/i) {	# for Dominique
	$AllInOneLine =~ s/<body.*?>/<body>/gsi;
}
else {				# default
	$AllInOneLine =~ s/<body.*?>/<body>$header_string/gsi;
	$AllInOneLine =~ s/<\/body>/$footer_string<\/body>/gsi;
}

# modify <table*> to get rid of all attributes and add border="0", cellspacing="1", cellpadding="5", & width="100%"
$AllInOneLine =~ s/<table.*?>/<table border=\"0\" cellspacing=\"1\" cellpadding=\"5\" width=\"100%\"> /gsi;

# modify <tr*> to get rid of all attributes
$AllInOneLine =~ s/<tr.*?>/<tr>/gsi;

# modify TD tags to get rid of all attributes except COLSPAN, ROWSPAN, and class for 508 encoding
#$AllInOneLine =~ s/<td( colspan=\"?[0-9]+\"?)?( rowspan=\"?[0-9]+\"?)?( class=\"?\w+\"?)?( align=\"?\w+\"?)?.*?>/<td$1$2$4>/gsi;	# keep for ref.
#$AllInOneLine =~ s/<td( height=\"?[0-9]+\"?)?( colspan=\"?[0-9]+\"?)?( rowspan=\"?[0-9]+\"?)?( class=\"?\w+\"?)?( align=\"?\w+\"?)?.*?>/<td$2$3$5>/gsi;

$AllInOneLine =~ s/(<td.*?)( height=\"?[0-9]+\"?)(.*?>)/$1$3/gsi;	# remove something like: height="29"

#$AllInOneLine =~ s/(<td.*?)( class=\"?\w+\"?)(.*?>)/$1$3/gsi;		# remove something like: class="x1244288"

 
#$AllInOneLine =~ s/<td( colspan=\"?[0-9]+\"?)?( rowspan=\"?[0-9]+\"?)?( align=\"?\w+\"?)?.*?>/<td$1$2$3>/gsi;
#$AllInOneLine =~ s/<td( class=\"?\w+\"?)?( colspan=\"?[0-9]+\"?)?( rowspan=\"?[0-9]+\"?)?( align=\"?\w+\"?)?.*?>/<td$1$2$3$4>/gsi;
#$AllInOneLine =~ s/<td( colspan=\"?[0-9]+\"?)?( rowspan=\"?[0-9]+\"?)?( class=\"?\w+\"?)?( align=\"?\w+\"?)?.*?>/<td$1$2$3$4>/gsi;
$AllInOneLine =~ s/<td( colspan=\"?[0-9]+\"?)?( rowspan=\"?[0-9]+\"?)?( class=\"?\w+\"?)?.*?>/<td$1$2$3>/gsi;


if($RunMode ne "silent") {   print "$AllInOneLine\n";  } 		# print if not in silent mode

#die "test point 77777\n";

# modify <td></td> or <td> </td> to <td>&nbsp;</td>
$AllInOneLine =~ s/(\240)*//gsi;		# remove " " which is not really a space.
$AllInOneLine =~ s/<td([^>]*)>\s*<\/td>/<td$1>&nbsp;<\/td>/gsi;

# =======================================================================================================================
# processing line by line to switch class name, change td to th for column head cells, and count max row/col level/layer.
# =======================================================================================================================
@lineset = split(/\n/, $AllInOneLine);
my $AllInOneLine_2;
my $mywholeline = "";
foreach $tmpline (@lineset) {
   $mywholeline .= $tmpline;
   #if( $tmpline !~ /<td .*?<\/td>/i ) {
   if( ($mywholeline =~ /<td .*>/i) && ($mywholeline !~ /.*<\/td>/i) ) {  # if <td>...</td> spread to several lines, reconstruct it
       #print "[$mywholeline]\n\n";
       #die "77\n";
       next;
   }
   
   # then save this whole line (most case <td> line) back to the $tmpline, and continue processing
   #if($tmpline ne $mywholeline) {  die "tmpline, mywholeline: \n$tmpline\n$mywholeline\n";  }
   
   $tmpline = $mywholeline; 
   if($tmpline =~ /class=\"?(\w+)\"?/i) {
     my $tmpmatchclass = $SClassEClass{$1};
     if ( defined($tmpmatchclass) ) { 		# if the class is defined in %SClassEClass
        #$tmpline =~ s/(.*)class=\"?\w+\"(.*)/$1class="$tmpmatchclass"$2/i;
        $tmpline =~ s/class=\"?\w+\"?/class="$tmpmatchclass"/;	# replace SClass with EClass
        #print "=>> $tmpline\n";
        #print "=> tmpmatchclass: $tmpmatchclass\n"; 
        &UpdateMaxCount($tmpmatchclass);
     }
     if( ($tmpmatchclass =~ /CHlevel/) || ($tmpmatchclass =~ /RHlevel/) ) { 	# for column head cells
        $tmpline =~ s/<td (.*)<\/td>/<th $1<\/th>/i;				# replace td with th
        #print "--- $tmpline\n";
     }
     elsif($tmpmatchclass =~ /RHlayer/ ) {
     	; 									# do nothing
     }
     else {
        
#     	if($tmpline =~ /<td class=\"?\w+\"?.*>(.*?)</ ) {	# other line with td and class, like <td class=xl784291 ...>
#     	    if($1 =~ /[1234567890,]+/) {
#     	       $tmpline =~ s/class=\"?\w+\"?/class=\"$NumberClass\"/i;		# use class for numbers, right align
#     	    }
#     	    else {
#     	       $tmpline =~ s/class=\"?\w+\"?/class=\"$StringClass\"/i;
#     	       print "=>> $tmpline\n";
#     	    }
#     	    #print "=>> $tmpline\n";
#     	    #die "5555\n";
#     	}
     	
     	
     	#if($tmpline =~ /<td class=\"?\w+\"?.*>(.*?)<\/td>/ ) {		# other line with td and class, like <td class=xl784291 ...>  
     	if($tmpline =~ /<td class=\"?\w+\"?.*?>(.*?)<\/td>/i ) {	# other line with td and class, like <td class=xl784291 ...>  
     	
     	    my $tmpvar1 = $1;
     	    
     	    if($RunMode ne "silent") {				# print if not in silent mode
     	       print "tmpline:  [$tmpline]\n\n";
     	       print "vari--1:  [$1]\n\n";
     	       print "tmpvar1:  [$tmpvar1]\n\n";
     	    }
     	    #die "test point 555666\n\n";
     	    
     	    
     	    while($tmpvar1 =~ /<(\w+)/i) {			# process the case of <td> content has tags: remove those tags
     	       if($RunMode ne "silent") {  print "tmpvar1-1: $tmpvar1\n"; } # print if not in silent mode
     	       $tmptag = $1;
     	       $tmpvar1 =~ s/<$tmptag.*?>(.*)<\/$tmptag>/$1/si;
     	       if($RunMode ne "silent") {  print "tmpvar1-2: $tmpvar1\n"; } # print if not in silent mode
     	       
     	       #die "8888-2\n\n";
     	    }
     	    
     	    if($tmpvar1 =~ /^[+-1234567890, ]+$/) {
     	       $tmpline =~ s/class=\"?\w+\"?/class=\"$NumberClass\"/i;		# use class for numbers, right align
     	    }
     	    else {
     	       $tmpline =~ s/class=\"?\w+\"?/class=\"$StringClass\"/i;
     	       #print "=>> $tmpline\n";
     	    }
     	    #if($tmpline =~ /ABX Air, Inc/i ) { die "new tmpline: $tmpline\n\n";}
     	}     	

     }
          
   }
   $AllInOneLine_2 .= $tmpline . "\n";
   $mywholeline = "";			# reset this tmp whole line storage to empty
} # end of foreach ()
#die "5555\n";
# =======================================================================================================================
# Add table508parameters line: ie <!--(table508parameters):MaxColHeadLevel,MaxRowHeadLevel,MaxRowHeadLayer:[1,1,2]-->
# =======================================================================================================================
my $T508ParaLine = "<!--(table508parameters):MaxColHeadLevel,MaxRowHeadLevel,MaxRowHeadLayer:[$MaxColHeadLevel,$MaxRowHeadLevel,$MaxRowHeadLayer]-->";
#die "$T508ParaLine \n";
$AllInOneLine_2 =~ s/(<table.*?>)/$1\n$T508ParaLine/i;



#print "... \n$AllInOneLine_2\n ...\n\n";
#die "test 666\n";

# ====== save the result to file ======
#print "All in one line: \n$AllInOneLine\nEnd of all in one line.\n";
close (OFN);	

#$OldFN2 = $OldFN."_mod";
#open(OFN, ">$OldFN2") || die "Can't open $OldFN2: $!\n";
#print NFN $AllInOneLine; 
print NFN $AllInOneLine_2; 
close (NFN);

if($RunMode ne "silent") {	# print if not in silent mode
   print "-------------------------------\n";
   print "Program  codecleaner.pl  end.  \n";
   print "-------------------------------\n\n";
}
####################################################################################

#=====================================================================
# &BuildMatchTable($WholeSet, "TableName"):
#---------------------------------------------------------------------
sub BuildMatchTable() {
    my ($myWholeSet, $myTableName) = @_;
    if($RunMode ne "silent") {	# print if not in silent mode
       print "\nin BuildMatchTable()... TalbeName: $myTableName\n\n";
    }
    while(length($myWholeSet) > 20) {
    	$myWholeSet =~ s/^(\.\w+\s.*?;})\s*(.*)$/$2/gsi;	# popup class one by one  
    	my $tmpclass = $1;
        #print "For myTableName: $myTableName, found one class: 1\n";
                
       if($myTableName eq 'SClassEClass')  {			# SClassEClass: background; Also set Table Start/Stop Class
    	   if($tmpclass =~ /background:#/) {
    	     if($tmpclass =~ /\.(\w+)\s.*background:#(\w{6});.*/s) {
    	       my $tmpSClass = $1;
    	       $SClassEClass{"$1"}=$SColorEClass{"$2"};		# using SColorEClass table build SClassEClass table
    	       #print "($1),($2), $SColorEClass{$2}\n";
    	       if($2 =~ /$TableStartColor/i) { $TableStartClass = $tmpSClass;  }	# set table start class
    	       if($2 =~ /$TableStopColor/i)  { $TableStopClass  = $tmpSClass;  }	# set table stop  class
    	     } 
    	   }
    	}
    	elsif($myTableName eq 'SClassBold' )  {			# SClassBold: for bold; 
    	   if($tmpclass =~ /\.(\w+)\s.*font-weight:700.*/s) {   $SClassBold{"$1"}="bold";   }
    	}
    	elsif($myTableName eq 'SClassItalic') {			# SClassItalic: for itlic; 
    	   if($tmpclass =~ /\.(\w+)\s.*font-style:italic.*/s) { $SClassItalic{"$1"}="italic";   }   	
    	}
        else {
          die "unknow parameter: $myTableName\n\n";
        }

    } # end of while
}

#=====================================================================
# &RemoveEmptyColumn($TmpDataTableRows):
#---------------------------------------------------------------------
sub RemoveEmptyColumn() {
   my ($mytable) = @_;
   #print "mytable: \n$mytable\n\n";
   my $newtable = "";
   
   # count empty column (from right side)
   my $NumberOfEmptyCol = 0;
   my @EmptyCellsInEachRow;
   my $tmptable = $mytable;
   while($tmptable =~ /<tr/ ) {	# count how many empty <td>s in each row and save the # in @EmptyCellsInEachRow
      $tmptable =~ s/(<tr .*?<\/tr>)(.*)$/$2/gsi;
      my $wholetr = $1;
      my $tmpcount = 0;
      while($wholetr =~ /<td/) {
         $wholetr =~ s/<td .*?>(.*?)<\/td>//si;
         if(length($1) == 0) { $tmpcount++;   }
         else                { $tmpcount = 0; }
      }
      push @EmptyCellsInEachRow, $tmpcount; 
      #print "== $tmpcount\n";
   }
   #push @EmptyCellsInEachRow, 7;
   #push @EmptyCellsInEachRow, 8;
   #push @EmptyCellsInEachRow, 27;
   
   @EmptyCellsInEachRow = sort { $a <=> $b } @EmptyCellsInEachRow; 
   $NumberOfEmptyCol = @EmptyCellsInEachRow[0];		# min_value in @EmptyCellsInEachRow

   if($RunMode ne "silent") {   	# print if not in silent mode
      print "\nEmptyCellsInEachRow: @EmptyCellsInEachRow\n";
      print "NumberOfEmptyCol: $NumberOfEmptyCol\n";
   }
   
   # now, remove empty columns 
   while($mytable =~ /<tr/ ) {
      $mytable =~ s/(^.*?)(<tr .*?<\/tr>)(.*)$/$3/si; 
      #$firstpart = $1;
      $trpart    = $2;
      #$lastpart  = $3;
      #print "firstpart,lastpart:    $firstpart, $lastpart\n"; 
      
      if($RunMode ne "silent") {  print "trpart-1: $trpart\n\n"; }    	# print if not in silent mode
      
      #if( $trpart =~ /^.*((<td .*?>(.*?)<\/td>\s*){$NumberOfEmptyCol})/si ) {
      #   my $tmptrpart = $1;
      #   die "tmptrpart: $tmptrpart\n\n";
      #   #die "in trpart processing block\n";
      #}
      
      #print "slow part start...\n";
      # The following line is working but too slow..., rewrite it using following while block
      #$trpart =~ s/(^.*)((<td .*?>.*?<\/td>\s*){$NumberOfEmptyCol})(.*$)/$1$4/si;
      $emptytddeleted = 0;
      while( ($trpart =~ /<td/i) && ($emptytddeleted < $NumberOfEmptyCol) ) {
         $trpart =~ s/(^.*)((<td .*?>.*?<\/td>\s*))(.*$)/$1$4/si;
         $emptytddeleted++;
      }
      #print "slow part stop...\n";
      #die "#1,#2,#3,#4: \n-$1\n -$2\n -$3\n -$4\n";
      
      $newtable = $newtable . $trpart;
      
      if($RunMode ne "silent") {  print "trpart-2: $trpart\n\n";  }  	# print if not in silent mode
      
      
      
      #die "newtable: \n\n$newtable\n";
   }
   
   if($RunMode ne "silent") {	print "newtable: \n\n$newtable\n";  } 	# print if not in silent mode
   
   #die "stop in RemoveEmptyColumn()\n";
   return $newtable;
}
#=====================================================================
# &UpdateMaxCount($tmpmatchclass):
#---------------------------------------------------------------------
sub UpdateMaxCount()  {
 	my ($mymatchclass) = @_;
	#print "mymatchclass: $mymatchclass\n";
	if($RunMode ne "silent") {	# print if not in silent mode
	   print "MaxColHeadLevel, MaxRowHeadLevel, MaxRowHeadLayer: $MaxColHeadLevel, $MaxRowHeadLevel, $MaxRowHeadLayer\n";
	}
	
	if( $mymatchclass =~ /CHlevel(\d)/ ) {
	   if($1 > $MaxColHeadLevel) { $MaxColHeadLevel = $1; }
	}	
	elsif(    $mymatchclass =~ /RHlevel(\d)/ ) {
	   if($1 > $MaxRowHeadLevel) { $MaxRowHeadLevel = $1; }
	}
	elsif( $mymatchclass =~ /RHlayer(\d)/ ) {
	   if($1 > $MaxRowHeadLayer) { $MaxRowHeadLayer = $1; }
	}
	
	if($RunMode ne "silent") {	# print if not in silent mode
	   print "MaxColHeadLevel, MaxRowHeadLevel, MaxRowHeadLayer: $MaxColHeadLevel, $MaxRowHeadLevel, $MaxRowHeadLayer\n";
	}
	#die "in mymatchclass()\n";
}
#=====================================================================
# CleanText($text):
#---------------------------------------------------------------------
sub CleanText() {
    my($tmptext) = @_;
    #print "$tmptext in CleanText()\n";
    
    # remove <span*>, </span>
    $tmptext =~ s/<span.*?>//gsi;
    $tmptext =~ s/<\/span>//gsi;
    
    # remove chr(160)
    #print "#160: $mychar\n";
    $mychar = chr(160);
    $tmptext =~ s/$mychar//gsi;
    
    return $tmptext;
}
#=====================================================================
# printInternalFile():
#---------------------------------------------------------------------
sub printInternalFile() {
	for $i (0..$#AllLines) {
		print "$i\t$AllLines[$i]";
	}
}
####################################################################################





#===============================================================================
__END__
