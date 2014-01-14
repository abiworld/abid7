#!/usr/bin/perl -w
# Name: TableProcess_508.pl
# Purpose: Process data table in html file(s) by adding id/headers attributes to make table 508 compatible.
#          It can used for a single file or a directory tree.
# Usage: (1) TableProcess_508.pl  DirectoryName
#        (2) TableProcess_508.pl  FileName
#        (3) TableProcess_508.pl -help
#        (4) TableProcess_508.pl -version
# History: 
# 	06/20/2011 Jinhui    (version 2.0)
#		changed to be run in drupal. add RunMode and set it to "silent". 
#	01/06/2006 Jinhui    (version 1.5)
#		debug to treat special case: rowspan in all row head columns
#		in BuildTablePropertyArray(): if($I > $MaxRowHeadLevel) { goto EndOfRowHeadCol; }
#		in Insert508Tags():           if($I > $MaxRowHeadLevel) { goto ProcessDataCells;}
#	11/29/2005 Jinhui    (version 1.4)
#		debug in BuildTablePropertyArray(): $TdArray[$J][($I+$LL)]{id} => $TdArray[$J][($I+1)]{id}
#	11/22/2005 Jinhui    (version 1.3)	
#		add RHlayer1_indent class to tell row head layer status
#	11/09/2005 Jinhui    (version 1.2)
#		add "<!--(table508parameters):MaxColHeadLevel,MaxRowHeadLevel,MaxRowHeadLayer:[1,1,2]-->" method
#		debug in BuildTablePropertyArray():  $TdArray[0][$J]{id} should be $TdArray[0][$tmpII]{id}
#	07/11/2005 Jinhui    (version 1.0)
#  		Original version 
# @copyrighted, exclusively for use at RITA/DOT only.
#
my $version = "2.0"; 

############ function list ###############################
# &ProcessDir($dirname);
# &ProcessFile($filename);
# &RewriteTableLines(*$newfile, $FirstLineNumber, $LastLineNumber, @AllLines);
# &Insert508Tags($FirstLineNumber, $LastLineNumber, @AllLines);
# &BuildTablePropertyArray($FirstLineNumber, $LastLineNumber, @AllLines);
# &WhichLayer($TDmatched);
# &Print2dArrayOfHash(@TwoDArrayOfHash);
# &resetArray();
# &CheckTableDimension($FirstLineNumber, $LastLineNumber, @AllLines); 
# &CheckTableType($FirstLineNumber, $LastLineNumber, @AllLines);
# &CopyNonTableLines(*$newfile, $FirstLineNumber, $LastLineNumber, @AllLines);
# &CanBeIgnored($filename, @AllLines);
# &StartEndLineNumbers($StartLineNumber, @AllLines);
############ Some global variables #######################
my $RunMode = "silent"; 	# options: silent (for drupal), interactive 
#my $RunMode = ""; 		# default: interactive

my $debug	= 0; 		# debugging mode: 1: more/additional output info than interactive mode

my $DataTableCount; 	
my $MaxColHeadLevel; 	# table parameter: when read cell data, how many headers to be read from column head (in same column) 
my $MaxRowHeadLevel; 	# table parameter: when read cell data, how many headers to be read from row head (in same row)
my $MaxRowHeadLayer;	# table parameter: when read cell data, how many headers to be read from row head (in different row)
my $InputMaxColHeadLevel = -1;	# default value of input (these 3 parameters not implemented yet)
my $InputMaxRowHeadLevel = -1; 	# default value of input
my $InputMaxRowHeadLayer = -1;	# default value of input
my $table508parameters = "N"; 	# parameters received from 1st line of each <table> in file: 
								# <!--(table508parameters) MaxColHeadLevel=1, MaxRowHeadLevel=1, MaxRowHeadLayer=1  -->
								# From this line values of MaxColHeadLevel, MaxRowHeadLevel, MaxRowHeadLayer are assigned.
								# default: N (not available)
my $TotalRow;		# number of rows in the table
my $TotalCol;		# number of columns in the table
my @TdArray;		# 2-d array of hash for presenting table property
my ($CNO, $RNO, $LNO);		# counters of Col Header, Row Header, and Laryer

##########################################################
use strict;
use File::Basename;    	# for using dirname()
use File::Path;        	# for using mkpath()
use File::Copy;		# for using copy()
use Time::localtime; 	# for time stamp in log file

$|=1;	# flush buffers 

########## check command line input, print help message if necessary
my $UsageMsg = "\tUsage 1: $0          DirectoryName       
	Usage 2: $0          FileName
	Usage 3: $0 -help
	Usage 4: $0 -version";
@ARGV == 1 or die  "\n$UsageMsg\n\n";  
if ($ARGV[0] =~ m/-h/i) { print "\n$UsageMsg\n\n";         exit; }
if ($ARGV[0] =~ m/-v/i) { print "\nVersion: $version\n\n"; exit; }

my $dirname  = $ARGV[0];		#    original directory name: dirname
my $filename = $ARGV[0];		# or original file      name: filename

########## create a log file for listing all the files changed.
my $ProcessLog = $ARGV[0]."_ProcessLog";
open (LogFile, ">>$ProcessLog") || die "ERROR: Can't open $ProcessLog: $!\n";
my $LF_fh = *LogFile; 	# to be used globally

########## get local time
my $tm = localtime;
if($RunMode ne "silent") {	# print if not in silent mode
   printf LogFile ("\n\n\n\n=================================================================\n");
   printf LogFile ("Start program   $0   at  %02d/%02d/%04d %02d:%02d:%02d\n", 
	$tm->mon+1, $tm->mday, $tm->year+1900, $tm->hour, $tm->min, $tm->sec);
   printf $LF_fh ("=================================================================\n");
}
if($RunMode ne "silent") {	# print if not in silent mode
   ########### A brief introduction before processing start.
   print "\n - This program is designed to process data tables in html file(s) for";
   print "\n   508 compliance. It can be used to process single file or a directory. \n";
   print "\n   In both cases, the original file/directory will not be changed.";
   print "\n   The results (including all files/directories that need no change) will be";
   print "\n   storied in 'directoryname'_NEW directory or, for the case of single file,";
   print "\n   in 'filename'_NEW. For more detailed information, please see file ReadMe.txt.\n";   
   print "\n - Files process info will be saved in file:  $ProcessLog\n"; 
   print "\n - Press \"Enter\" key to continue (or Ctrl+c to quit) ..... ";
   my $answer = <STDIN>;
   sleep 1;	# wait 1 second, for allow possible Ctrl+c input.
}

########### check command line parameter: process file or directory
my $IsDirectory;
if(! -d $dirname) {		# if it is not a directory
	$IsDirectory = 0;	# 0: it's a file.
}
else {				# if it is a directory
	$IsDirectory = 1; 	# 1: it's a directory
}

if($RunMode ne "silent") {	# print if not in silent mode
   print "\n=================================================================\n";
   printf ("Start program   %s   at  %02d/%02d/%04d %02d:%02d:%02d\n", $0, $tm->mon+1, $tm->mday, $tm->year+1900, $tm->hour, $tm->min, $tm->sec);
   print   "=================================================================\n";
}

if($IsDirectory) {	# for directory
	if($RunMode ne "silent") {	# print if not in silent mode
	   print         "\n==== Starting from directory: $dirname ====\n";
	   print LogFile "\n==== Starting from directory: $dirname ====\n";
	}
	&ProcessDir($dirname);
}
else {			# for file
	#print "\n**** Starting processing file: [$filename] ****\n\n";
	&ProcessFile($filename);
}

if($RunMode ne "silent") {	# print if not in silent mode
   print         "\n=================================================================\n";
   print $LF_fh  "\n=================================================================\n";
   printf        ("End of the program                    at  %02d/%02d/%04d %02d:%02d:%02d\n",  $tm->mon+1, $tm->mday, $tm->year+1900, $tm->hour, $tm->min, $tm->sec); 
   printf $LF_fh ("End of the program                    at  %02d/%02d/%04d %02d:%02d:%02d\n",  $tm->mon+1, $tm->mday, $tm->year+1900, $tm->hour, $tm->min, $tm->sec);
   print         "=================================================================\n";
   print $LF_fh  "=================================================================\n";
}

close (LogFile); 
#####################################################################################
# ProcessDir($dirname)
#
#------------------------------------------------------------------------------------
sub ProcessDir(){
	my ($dirname)=@_;
	my ($file, $my_status, $new_dirname, @files);
	
	# copy the directory (recursively)
	#print "\ndirname: $dirname";
	($new_dirname = $dirname) =~ s/^($ARGV[0])/$1_NEW/i;
	#print "\ndirname: $dirname new_dirname: $new_dirname\n\n";
	mkpath($new_dirname, 1, 0777); 
	
		
	opendir(DIR, $dirname) or die "ERROR: Can't open $dirname: $!";
	@files = readdir(DIR);

	foreach $file (@files) {
		#if($oldFN =~ m/^\.{1,2}$/) {;;;}	## don't process . and .. files
		if ( (-d "$dirname/$file") && ($file ne ".") && ($file ne "..") ) { ## for directory
			&ProcessDir("$dirname/$file");		# called recursively.
		}
		elsif (-T "$dirname/$file"){		## for Text file
		        $my_status = &ProcessFile("$dirname/$file");
			$my_status == 0 or die "\n*** ERROR: ProcessFile() stopped abnormally!\n\n";
		}
		else {					## for others, simple make a copy 
			copy("$dirname/$file", "$new_dirname/$file");
		}
	}
	closedir(DIR);
}
#####################################################################################
# ProcessFile($filename)
#------------------------------------------------------------------------------------
sub ProcessFile() {
	my ($filename) = @_;		# original source file
	my $FoundBar 		= 0;
	my $FoundLocation 	= 0;
	my ($NewFileName, $line);
	my $MarkerLine = "";
	my ($new_filename);
	$DataTableCount = 0;		# count how many data tables in this file
	
	open(OFN,   "$filename" ) || die "Can't open $filename: $!\n";
	my @AllLines = <OFN>; 		# get the content of original file
	close (OFN);
	
	if($RunMode ne "silent") {	# print if not in silent mode
	   print "\n**** Starting processing file: [$filename] ****\n\n";
	   print $LF_fh "\n**** Starting processing file: [$filename] ****\n\n";
	}	
	
	# seperate dirname and filename:  
	my $ThisBaseName = basename($filename);
	my $ThisDirName  = dirname($filename);

	# create new file name:
	if($ThisDirName eq ".") {
		$NewFileName = $filename."_NEW"; 	# for single file
	}
	else {
		#hjh mkpath($ThisDirName."_NEW/", 1, 0777);
		#hjh $NewFileName = $ThisDirName."_NEW/".$ThisBaseName;
		($NewFileName = $ThisDirName."/".$ThisBaseName) =~ s/^($ARGV[0])/$1_NEW/i;
	} 
	
	if(&CanBeIgnored($filename, @AllLines)) {	# check if the file should be skipped
		copy("$filename", "$NewFileName"); 	# simply make a copy
		return 0;				# return: No more precess necessary; 0: normal/continue
	}
	
	my ($tmpSLN, $tmpELN, $tmpStartLineNumber);
	$tmpStartLineNumber = 0;
	open(NF_fh,   ">$NewFileName" ) || die "Error: Can't open $NewFileName: $!\n";
	$CNO = $RNO = $LNO = -1; 	# counter of Col Header, Row Header, and Laryer


	do {	# using this loop to process data table one by one
	   ($tmpSLN, $tmpELN) = &StartEndLineNumbers($tmpStartLineNumber, @AllLines);
	   
	   if (($tmpSLN != 99999) && ($tmpELN != 99999)) { 	# found next data table
	      $DataTableCount++;
	      
	      if($RunMode ne "silent") {	# print if not in silent mode
	         print        " Data Table ($DataTableCount)  ---------------------------------------------\n";
	         print $LF_fh " Data Table ($DataTableCount)  ---------------------------------------------\n";
	         print        " StartLN, EndLN    :   $tmpSLN, $tmpELN\n";
	         print $LF_fh " StartLN, EndLN    :   $tmpSLN, $tmpELN\n";
	      }
	      &CopyNonTableLines(*NF_fh, $tmpStartLineNumber, $tmpSLN-1, @AllLines);
	      &RewriteTableLines(*NF_fh, $tmpSLN,               $tmpELN, @AllLines);
	   }
	   else {						# no more data table 
	      &CopyNonTableLines(*NF_fh, $tmpStartLineNumber, $tmpSLN-1, @AllLines);
	      #&RewriteTableLines(*NF_fh, $tmpSLN,               $tmpELN, @AllLines);
	   }
		
	   $tmpStartLineNumber = $tmpELN + 1;
	} while (($tmpSLN != 99999) && ($tmpELN != 99999));
	
	#close (NF_fd);
	#hjh for testing only
	#hjh copy("$filename", "$NewFileName"); 
	
	return 0;
}
#####################################################################################
# RewriteTableLines(*$newfile, $FirstLineNumber, $LastLineNumber, @AllLines)
#	this function is used to add id/headers markers to the data table
#	input: 1. new file name; 2. 1st line # of the data table; 3. last line # of the table
#	       4. all lines of the original files.
#	output: modified table will be write to the new file, and 
#		return code 0: success; or 1: error
#------------------------------------------------------------------------------------
sub RewriteTableLines() {
	my ($tmpNF_fh, $tmpFLN, $tmpLLN, @tmpAllLines) = @_;
	#print "tmpNF_fh,tmpFLN,tmpLLN,tmpAllLines: $tmpNF_fh, $tmpFLN, $tmpLLN, @tmpAllLines\n";
	my $line;	# temp variable
	
	# check the dimensions of the table, (row x col)
	&CheckTableDimension($tmpFLN, $tmpLLN, @tmpAllLines);
	
	if($RunMode ne "silent") {	# print if not in silent mode
	   print        " TotalRow, TotalCol:   $TotalRow, $TotalCol\n";
	   print $LF_fh " TotalRow, TotalCol:   $TotalRow, $TotalCol\n";
	}
	
	# check & get tabel type parameters: MaxColHeadLevel, MaxRowHeadLevel, MaxRowHeadLayer.
	$MaxColHeadLevel = $MaxRowHeadLevel = $MaxRowHeadLayer = -1;
	#print "MaxColHeadLevel, MaxRowHeadLevel, MaxRowHeadLayer: $MaxColHeadLevel, $MaxRowHeadLevel, $MaxRowHeadLayer\n";
	&CheckTableType($tmpFLN, $tmpLLN, @tmpAllLines);
	
	if($RunMode ne "silent") {	# print if not in silent mode
	   print        " MaxColHeadLevel, MaxRowHeadLevel, MaxRowHeadLayer:   $MaxColHeadLevel, $MaxRowHeadLevel, $MaxRowHeadLayer\n\n";
	   print $LF_fh " MaxColHeadLevel, MaxRowHeadLevel, MaxRowHeadLayer:   $MaxColHeadLevel, $MaxRowHeadLevel, $MaxRowHeadLayer\n\n";
	}
	
	# build table property array:
	#print $tmpNF_fh "<!-- ========== Data Table ($DataTableCount) ========== -->\n";
	&BuildTablePropertyArray($tmpFLN, $tmpLLN, @tmpAllLines);

	
	# insert id/headers 508 tags and get the result in a string, then write it to the new file:
	print $tmpNF_fh "<!-- ========== Data Table ($DataTableCount) start ========== -->\n";;	
	my $tmpNewTable = &Insert508Tags($tmpFLN, $tmpLLN, @tmpAllLines);
	print $tmpNF_fh "$tmpNewTable";
	print $tmpNF_fh "<!-- ========== Data Table ($DataTableCount) stop ========== -->\n";;

	#exit;
}
#####################################################################################
# Insert508Tags($FirstLineNumber, $LastLineNumber, @AllLines)
#
#------------------------------------------------------------------------------------
sub Insert508Tags() {
	my ($tmpFLN, $tmpLLN, @tmpAllLines) = @_;
	my ($II, $mystring);
	my $mynewstring = "";	# for new/modified table lines
	
	for ($II=$tmpFLN; $II<=$tmpLLN; $II++) {  # combine all lines into one string
	   $mystring .= $tmpAllLines[$II];
	}
	
	my $J = 0;	# <tr> counter
	
	# first, process colhead line(s):
	if($MaxColHeadLevel > 0) {	      
	   while( $mystring =~ m/(<tr.*?>)(.*?)(<\/tr>)/si ) {  # for each header line(s)
	      $mystring = $';	# continue to process the REST of <table>
	      $J++;
	      my $I = 0; 	# <td> counter
	      my $tmpTR = $2;	# content of this <TR>: everything between <TR> and </TR>
	      my $tmpTRe = $3;	# end part, to be added to $mynewstring later
	      $mynewstring .= $`.$1;
	      
	      while( $tmpTR =~ m/<t(h|d)(.*?)>(.*?)<\/t\1>/si ) {	# loop each <td>
	         $tmpTR = $';	# keep the REST of <tr> for next match
	         $I++;
	         my $tmpTDt = $2; 	# content (tags) in this <TD> or <TH> 
	         my $tmpTD  = $3;
	         #print "\nTesting td,tr: ($tmpTDt),($tmpTR)\n";
	         
	         while( defined($TdArray[$J][$I]{rowspan}) ) { $I++; }  # skip the rowspaned <td>
	         $mynewstring .= $`."<t".$1.' id="'.$TdArray[$J][$I]{id}.'"'.$2.">".$3."</t".$1.">";

	         if($tmpTDt =~ /colspan=('|")?(\d*)('|")?/ ) {	# for n colspan: mark next n-1 cols
	            my $tmpCNT = $2 + 0; 
	            for (my $LL=1; $LL<$tmpCNT; $LL++) {  $I++;  }
	         } 
	         
	      }  ### EO  while( $tmpTR =~ ......
	      $mynewstring .= $tmpTR;	# add what has been left, like cr.
	      
	      if($RunMode ne "silent") {	# print if not in silent mode
	         if($debug && ($J == $MaxColHeadLevel) ) { print "mynewstring[$J][$I]: \n$mynewstring\n"; }
	      }
	      
	      $mynewstring .= $tmpTRe;	# add the end of matching <tr> (</tr>)
	      if($J >= $MaxColHeadLevel) {last;}
	   }
	   
	}  ### EO if($MaxColHeadLevel > 0)
	
	
	# then, process table data lines:
	while( $mystring =~ m/(<tr)(.*?)>(.*?)(<\/tr>)/si ) { 	# loop each <tr>
	   $mystring = $';	# save it to process the REST of <table>, later
	   $J++;		# <tr> counter
	   
	   my $I = 0;
	   my $tmpTRb= $1;	# begin: <TR
	   my $tmpTRt= $2;	# content (tags) in this <TR>
	   my $tmpTR = $3;	# content of this <TR>: everything between <tr> & </tr>
	   my $tmpTRe= $4;	# end:   </TR>
	   $mynewstring .= $`.$1.$2.">";

	   while( $tmpTR =~ m/(<t(h|d))(.*?)>(.*?)(<\/t\2>)/si ) {	# loop each <td>
	      $tmpTR = $';	# keep the REST of <tr> for next match
	      $I++;		# <td> counter
	      my $tmpTDprifix = $`;
	      my $tmpTDb = $1;	# begin: <TD of <TH
	      my $tmpTDt = $3; 	# content (tags) in this <TD> or <TH> 
	      my $tmpTD  = $4;	# content of this <TD> or <TH>
	      my $tmpTDe = $5;	# end:   </TD> or </TH>
	      my $tmpTDmatched = $&; 	# whole match

	      
	      
	      if($I <= $MaxRowHeadLevel) {	# if in row head column(s): 
	         while( defined($TdArray[$J][$I]{rowspan}) ) { 	# skip the rowspaned <td>
	            $I++; 
	            if($I > $MaxRowHeadLevel) { goto ProcessDataCells;}		# if this td is already a data cell
	         }	

	         #print "***tmpTDprifix,tmpTDb,tmpTDt,tmpTD,tmpTDe: $tmpTDprifix,$tmpTDb,$tmpTDt,$tmpTD,$tmpTDe\n";
	         #print "****TdArray[$J][$I]{id}: $TdArray[$J][$I]{id}\n";
	         
	         $mynewstring .= $tmpTDprifix.$tmpTDb.' id="'.$TdArray[$J][$I]{id}.'" '.$tmpTDt.">".$tmpTD.$tmpTDe;

	         if($tmpTDt =~ /colspan=('|")?(\d*)('|")?/ ) {	# for n colspan: mark next n-1 cols
	            my $tmpCNT = $2 + 0; 
	            for (my $LL=1; $LL<$tmpCNT; $LL++) { $I++; }
	         }

	      }  ### EO if($I <= $MaxRowHeadLevel)
	      else {				# if not in row head column(s): 
	         ProcessDataCells: {;}
	         # assumn no colspan in data cell.
	         
	         $TdArray[$J][$I]{id} = $TdArray[$J][0]{id}." ".$TdArray[0][$I]{id};
	         if( ($tmpTD =~ m/^\s*$/) || ($tmpTD =~ m/^\s*&nbsp;\s*$/) ) {	# if this is an empty cell
	            $mynewstring .= $tmpTDprifix.$tmpTDmatched;
	         }
	         else {
	            $mynewstring .= $tmpTDprifix.$tmpTDb.' headers="'.$TdArray[$J][$I]{id}.'" '.$tmpTDt.">".$tmpTD.$tmpTDe;
	         }
	      }

	   }  ### EO while( $tmpTR =~ m/(<t(h|d))(.*?)>(.*?)(<\/t\2>)/si )
	   
	   $mynewstring .= $tmpTR;	# add what has been left, like cr.
	   $mynewstring .= $tmpTRe;
	   
	}  ### EO while( $mystring =~ m/(<tr)(.*?)>(.*?)(<\/tr>)/si )
	if($RunMode ne "silent") {	# print if not in silent mode
	   if($debug) {print "\nEnd of Insert508Tags():\n"; &Print2dArrayOfHash(@TdArray);}
	}
	#print "mystring<:> \n$mystring\n";
	$mynewstring .= $mystring;
	if($RunMode ne "silent") {	# print if not in silent mode
	   if($debug) { print "mynewstring<:> \n$mynewstring\n"; }
	}
	return $mynewstring;
}
#####################################################################################
# BuildTablePropertyArray($FirstLineNumber, $LastLineNumber, @AllLines)
#
#------------------------------------------------------------------------------------
sub BuildTablePropertyArray() {
	my ($tmpFLN, $tmpLLN, @tmpAllLines) = @_;
	my ($II, $mystring, $tmpLayerNumber);
	my @tmpLayerID = ("","","","","","","","","","",""); 	# total 11 elements, but reserve [0] 
	#my $myDataTableLines = "";
	
	&resetArray();	# reset table property array
	
	for ($II=$tmpFLN; $II<=$tmpLLN; $II++) {  # combine all lines into one string
	   $mystring .= $tmpAllLines[$II];
	}
	
	my $J = 0;	# <tr> counter
	
	# process header lines 
	if($MaxColHeadLevel > 0) {	      
	   while( $mystring =~ m/<tr(.*?)>(.*?)<\/tr>/si ) {  # for each header line(s)
	      $mystring = $';	# continue to process the REST of <table>
	      $J++;
	      my $I = 0; 	# <td> counter
	      my $tmpTR = $2;	# content of this <TR>: everything between <TR> and </TR>
	      
	      while( $tmpTR =~ m/<t(h|d)(.*?)>(.*?)<\/t\1>/si ) {	# loop each <td> or <th>
	         $tmpTR = $';	# keep the REST of <tr> for next match
	         $I++;
	         my $tmpTDt = $2; 	# content (tags) in this <TD> or <TH> 
	         my $tmpTD  = $3;
	         #print "\nTesting td,tr: ($tmpTDt),($tmpTR)\n";
	         
	         while( defined($TdArray[$J][$I]{rowspan}) ) { $I++; }  # skip the rowspaned <td>
	         $CNO++;	# counter for Column head 
	         $TdArray[$J][$I]{id} = "CH" . $CNO;
	         #print "-----i,CNO,TD: [$I][$CNO][$tmpTD]\n";      
	         	         
	         if($tmpTDt =~ /rowspan=('|")?(\d*)('|")?/ ) {	# for n rowspan: mark next n-1 rows
	            my $tmpCNT = $2 + 0; 	# a temp count 
	            #if($2 ne "2")    {die "Found error in rowspan...[$2]\n";}
	            #if($tmpCNT != 2) {die "found error in rowspan...[$tmpCNT]\n";}
	            
	            #die "try1 tmpCNT,(int)tmpCNT: [$tmpCNT] [int($tmpCNT)]\n";
	            
	            for (my $LL=1; $LL<$tmpCNT; $LL++) {
	               #$TdArray[($J+$LL)][$I]{id} = $TdArray[$J][$I]{id};
	               $TdArray[($J+$LL)][$I]{rowspan} = 1;
	            }
	         }
	         
	         if($tmpTDt =~ /colspan=('|")?(\d*)('|")?/ ) {	# for n colspan: mark next n-1 cols
	            my $tmpCNT = $2 + 0; 
	            #die "try2 tmpCNT,(int)tmpCNT: $tmpCNT, int($tmpCNT)";
	            
	            for (my $LL=1; $LL<$tmpCNT; $LL++) {
	               #$TdArray[$J][($I+$LL)]{id} = $TdArray[$J][$I]{id};
	               #$TdArray[$J][($I+$LL)]{colspan} = 1;
	               $TdArray[$J][($I+1)]{id} = $TdArray[$J][$I]{id};
	               $TdArray[$J][($I+1)]{colspan} = 1; 
	               $I++;
	               #print "J,I: [$J][$I]\n";
	            }
	         }
	         #print "here.... I:  [$I][$tmpTD]\n"; 
	                
	      }  ### EO  while( $tmpTR =~ ......
	      
	      if($RunMode ne "silent") {	# print if not in silent mode
	      if( $debug && ($I != $TotalCol) ) { # this step is a consistence test only. So, should turn off in production.
	         for (my $LL=$I; $LL<=$TotalCol; $LL++) {	# count the effect of row span
	            if( defined($TdArray[$J][$LL]{rowspan}) && ($TdArray[$J][$LL]{rowspan} == 1) ) {
	               $I++;
	               print "  Here,  adjusted I:  $I\n";
	            }
	         }
	         if($I != $TotalCol) {die "\nError: found $I <> $TotalCol, (max col num)\n";}
	      }
	      } # end of RunMode block
	      
	      if($J >= $MaxColHeadLevel) {last;}
	   }
	   
	}  ### EO if($MaxColHeadLevel > 0)
	if($RunMode ne "silent") {	# print if not in silent mode
	   if($debug) {&Print2dArrayOfHash(@TdArray);}
	}
	#die " stop @ testing point 210b\n";
	
	# combine the column head 508 tags and save it to $TdArray[0][II]{id}
	for(my $tmpJJ=1; $tmpJJ<=$MaxColHeadLevel; $tmpJJ++) {
	   for(my $tmpII=1; $tmpII<=$TotalCol; $tmpII++) {
	      if($tmpJJ == 1) {
	         $TdArray[0][$tmpII]{id} = $TdArray[$tmpJJ][$tmpII]{id};
	      }
	      else {
	         if(defined($TdArray[$tmpJJ][$tmpII]{rowspan}) && ($TdArray[$tmpJJ][$tmpII]{rowspan} == 1) ) {
	            ;		# ignore row span
	         }
	         else {
	            $TdArray[0][$tmpII]{id} = $TdArray[0][$tmpII]{id} . " " . $TdArray[$tmpJJ][$tmpII]{id};
	         }
	      }
	   }
	}
	if($RunMode ne "silent") {	# print if not in silent mode
	   if($debug) {&Print2dArrayOfHash(@TdArray);}
	}
	#die " stop @ testing point 211\n";
	
	# continue, process table data line
	while( $mystring =~ m/(<tr)(.*?)>(.*?)(<\/tr>)/si ) { 	# loop each <tr>
	   $mystring = $';	# save it to process the REST of <table>, later
	   $J++;		# <tr> counter
	   
	   my $I = 0;
	   my $tmpTRb= $1;	# begin: <TR
	   my $tmpTRt= $2;	# content (tags) in this <TR>
	   my $tmpTR = $3;	# content of this <TR>: everything between <tr> & </tr>
	   my $tmpTRe= $4;	# end:   </TR>
	   #print " [$`$tmpTRb$tmpTRt>$tmpTR$tmpTRe]\n";
	   #die " stop @ testing point 234\n";
	   
	   while( $tmpTR =~ m/(<t(h|d))(.*?)>(.*?)(<\/t\2>)/si ) {	# loop each <td>
	      $tmpTR = $';	# keep the REST of <tr> for next match
	      $I++;		# <td> counter
	      my $tmpTDb = $1;	# begin: <TD of <TH
	      my $tmpTDt = $3; 	# content (tags) in this <TD> or <TH> 
	      my $tmpTD  = $4;	# content of this <TD> or <TH>
	      my $tmpTDe = $5;	# end:   </TD> or </TH>
	      my $tmpTDmatched = $&; 	# whole match
	      #print " [$`$tmpTDb$tmpTDt>$tmpTD$tmpTDe]\n";
	      #die " stop @ testing point 301\n";
	      
	      # if in row head column(s): 
	      if($I <= $MaxRowHeadLevel) {
	         while( defined($TdArray[$J][$I]{rowspan}) ) { 		# skip the rowspaned <td>
	            $I++; 
	            if($I > $MaxRowHeadLevel) { goto EndOfRowHeadCol; } #for special case: all row head level have rowspan
	         }	
	         $RNO++;	# counter for Row head 
	         
	         # if this is the 1st <td> or <th>: 
	         if($I == 1) {
	            $tmpLayerNumber = &WhichLayer($tmpTDmatched); 	# check which layer it is in
	            $TdArray[$J][0]{layer} = $tmpLayerNumber;		# keep it in TdArray[J][0]{layer}
	            if($RunMode ne "silent") {	# print if not in silent mode
	               if($debug) { print " Row $J: tmpLayerNumber:  $tmpLayerNumber\n"; }
	            }
	            #die " stop @ testing point 354\n";
	         
	            #$TdArray[$J][0]{id} = $TdArray[0][$I]{id}; 
	            #if($tmpLayerNumber == 1) {
	            #   $TdArray[$J][$I]{id} = "RH" . $
	            #}
	         
	         }
	         else {
	            $tmpLayerNumber = $TdArray[$J][0]{layer}; 
	            #$TdArray[$J][0]{id} = $TdArray[$J][0]{id} . " " . $TdArray[0][$I]{id};
	         }  ### EO if($I == 1)
	      
	         $LNO = $tmpLayerNumber;	# counter for row head Layer
	         $TdArray[$J][$I]{id} = "RH" . $LNO . "-" . $RNO;
	         
	         # create row head layer id
	         if($I == 1) {	# only do it when processing 1st col.   (what happen, if has colspan??????)
	         	
	            for(my $KK=1; $KK<=$MaxRowHeadLayer; $KK++) {
	               if($KK == $LNO) { $tmpLayerID[$KK] = $TdArray[$J][$I]{id}; }	# add/reset this layer,upper layer will be kept
	               if($KK >  $LNO) { $tmpLayerID[$KK] = ""; }			# clear down layer
	            }
	            
	            for(my $KK=1; $KK<$LNO; $KK++) {	# combine the layer IDs and save them in @TdArray[J][0] {layerid}
	               
	               if($KK == 1) {
	                  if( defined($tmpLayerID[1]) ) {$TdArray[$J][0]{layerid} = $tmpLayerID[1];}
	                  else                          {$TdArray[$J][0]{layerid} = "";}
	               }
	               else {
	                  $TdArray[$J][0]{layerid} = $TdArray[$J][0]{layerid} . " " . $tmpLayerID[$KK];
	                  if($RunMode ne "silent") {	# print if not in silent mode
	                     print "==> I,TdArray[$J][0]{layerid}: $I,$TdArray[$J][0]{layerid}\n";
	                  }
	               }
	               
	            }
	          
	         }
	         
	      
	         if($tmpTDt =~ /rowspan=('|")?(\d*)('|")?/ ) {	# for n rowspan: mark next n-1 rows
	            my $tmpCNT = $2 + 0; 
	            #die "try1 tmpCNT,(int)tmpCNT: [$tmpCNT] [int($tmpCNT)]\n";
	            for (my $LL=1; $LL<$tmpCNT; $LL++) {
	               $TdArray[($J+$LL)][$I]{rowspan} = 1;
	               $TdArray[($J+$LL)][$I]{id} = $TdArray[$J][$I]{id};
	               if($I ==1) { $TdArray[$J+$LL][0]{layer} = $TdArray[$J][0]{layer}; }
	            }
	         }
	         
	         if($tmpTDt =~ /colspan=('|")?(\d*)('|")?/ ) {	# for n colspan: mark next n-1 cols
	            my $tmpCNT = $2 + 0; 
	            #die "try2 tmpCNT,(int)tmpCNT: $tmpCNT, int($tmpCNT)"; 
	            for (my $LL=1; $LL<$tmpCNT; $LL++) {
	               #$TdArray[$J][($I+$LL)]{colspan} = 1;
	               #$TdArray[$J][($I+$LL)]{id} = $TdArray[$J][$I]{id};
	               $TdArray[$J][($I+1)]{colspan} = 1;
	               $TdArray[$J][($I+1)]{id} = $TdArray[$J][$I]{id};	               
	               $I++;
	               #print "J,I: [$J][$I]\n";
	            }
	         }
	         
	         EndOfRowHeadCol: {;}
	         # combine the row head (include related col head) 508 tags and save it to $TdArray[J][0]{id}
	         if($I >= $MaxRowHeadLevel) {	# do it only at the end of processing row head
	            for(my $tmpII=1; $tmpII<=$MaxRowHeadLevel; $tmpII++) {
	               if($tmpII == 1) {
	                  $TdArray[$J][0]{id} = $TdArray[0][$tmpII]{id};	# related col headers id                 
	                  
	                  #print "TdArray[$J][0]{id}:::: $TdArray[$J][0]{id}\n";
	                  #print "TdArray[$J][0]{layerid}:: $TdArray[$J][0]{layerid}\n";
	                  
	                  if(!defined($TdArray[$J][0]{layerid})) {$TdArray[$J][0]{layerid}="";}
	                  $TdArray[$J][0]{id} = $TdArray[$J][0]{id} . " " . $TdArray[$J][0]{layerid};	# related row head layer id 
	                  $TdArray[$J][0]{id} = $TdArray[$J][0]{id} . " " . $TdArray[$J][$tmpII]{id};	# this row head id 
	               }
	               else {
	                  if(defined($TdArray[$J][$tmpII]{colspan}) && ($TdArray[$J][$tmpII]{colspan} == 1) ) {
	                     ;
	                  }
	                  else {
	                     #print "====> tmpII,TdArray[J][0]{id},TdArray[0][J]{id},TdArray[J][tmpII]{id}: [$tmpII],[$TdArray[$J][0]{id}],[$TdArray[0][$tmpII]{id}],$TdArray[$J][$tmpII]{id}\n"; 
	                     $TdArray[$J][0]{id} = $TdArray[$J][0]{id} . " " . $TdArray[0][$tmpII]{id};	# related col headers id
	                     $TdArray[$J][0]{id} = $TdArray[$J][0]{id} . " " . $TdArray[$J][$tmpII]{id};
	                  }
	               }
	               
	            }
	         
	         }  ### EO if($I == $MaxRowHeadLevel)
	         	         	      
	      }  ### EO if($I <= $MaxRowHeadLevel)
	      #EndOfRowHeadCol: {;}	      

	   }  ### EO while( $tmpTR =~ m/(<t(h|d))(.*?)>(.*?)(<\/t\2>)/si )
	   
	}  ### EO while( $mystring =~ m/(<tr)(.*?)>(.*?)(<\/tr>)/si )
	if($RunMode ne "silent") {	# print if not in silent mode
	   if($debug) {print "\nEnd of BuildTablePropertyArray():\n"; &Print2dArrayOfHash(@TdArray);}
	}
	
	
	# if J != total row number die
	die "J != total row number!\n" if($J != $TotalRow);
	#die "reached end of .....\n";
}

#####################################################################################
# WhichLayer($TDmatched)
#	check the whole <td> (or <th>) and decide which layer it is in
#------------------------------------------------------------------------------------
sub WhichLayer() {
	my ($tmpTDmatched) = @_;
	
	# if MaxRowHeadLayer is 1, then all row header is in layer one.
	if($MaxRowHeadLayer ==1) {
	  return 1;
	}
	
	# check if css class is used to indicate layer, if yes use it:
	if($tmpTDmatched =~ /.*?class=.?RHlayer(\d).*/si) { 
	   return $1;
	}
	
	if($tmpTDmatched =~ m/<th/si) {
	   return 1;
	}
	elsif($tmpTDmatched =~ m/<td/si) {
	   return 2;
	}
	else {
	   die "Error in WhichLayer(): exception of <th>/<td>\n";
	}
}
#####################################################################################
# Print2dArrayOfHash(@TwoDArrayOfHash) 
#------------------------------------------------------------------------------------
sub Print2dArrayOfHash(){
	my (@Array) = @_;
	my ($i, $href, $key);
	for $i (0..$#Array) {
		my $j = 0;
		for $href (@{$Array[$i]}) {
			print " row=$i, col=$j\t { ";
			for $key (keys %$href ) {	
				print "$key=$href->{$key} ";
			} 
			print "}\n";
			$j++;
		} 
		print "\n";
	}
	print "\n";
}
#####################################################################################
# resetArray(): 
#	reset  TdArray[][]
#------------------------------------------------------------------------------------
sub resetArray(){
	my $i;
	for $i (0...$#TdArray) 	{ pop(@TdArray); }
	# how about @Array = (); (see p.39)
}
#####################################################################################
# CheckTableDimension($FirstLineNumber, $LastLineNumber, @AllLines) 
#	This function is used to find the row and col numbers of the table.
#------------------------------------------------------------------------------------
sub CheckTableDimension() {
	my ($tmpFLN, $tmpLLN, @tmpAllLines) = @_;
	my ($II, $mystring);
	for ($II=$tmpFLN; $II<=$tmpLLN; $II++) {  # combine all lines into one string
	  $mystring .= $tmpAllLines[$II];
	}

	my $J = 0;
	my $MaxCol = 0;
	while( $mystring =~ m/<tr(.*?)>(.*?)<\/tr>/si ) { 	# loop each <tr>
	   $mystring = $';	# continue to process the REST of <table>
	   $J++;
	   
	   my $K = 0;
	   my $tmpTR = $2;	# content of this <TR>
	   while( $tmpTR =~ m/<t(h|d)(.*?)>(.*?)<\/t\1>/si ) {	# loop each <td>
	      $tmpTR = $';	# continue to process the REST of <tr>
	      $K++;
	   }
	   if($K > $MaxCol) { $MaxCol = $K;}
	}
	
	$TotalRow = $J;
	$TotalCol = $MaxCol;
}
#####################################################################################
# CheckTableType($FirstLineNumber, $LastLineNumber, @AllLines)
#		Check table content to decide 
#		$MaxColHeadLevel, $MaxRowHeadLevel, $MaxRowHeadLayer.
#
sub CheckTableType() {
	my ($tmpFLN, $tmpLLN, @tmpAllLines) = @_;
	my ($II, $mystring);
	my $N_colhead      = 0;
	my $N_rowhead      = 0;
	my $N_rowheadlevel = 0;
	my $N_rowheadlayer = 0;
	my $N_rowheadlayerth=0;
	my $N_rowheadlayertd=0;
	
	for ($II=$tmpFLN; $II<=$tmpLLN; $II++) { 
		$mystring .= $tmpAllLines[$II]; 
		
		# check to see if parameter line can be found. If yes, use it
		# Format: <!--(table508parameters):MaxColHeadLevel,MaxRowHeadLevel,MaxRowHeadLayer:[1,1,2]-->
		#
		if ($tmpAllLines[$II] =~ /(table508parameters)/)  {
			$table508parameters = $tmpAllLines[$II];
			$table508parameters =~ s/<!--\(table508parameters\):.*?\[(.*?)\]\s*-->/$1/;
			($MaxColHeadLevel, $MaxRowHeadLevel, $MaxRowHeadLayer) = split(',', $table508parameters);
			$table508parameters = "N";	# reset to default
			return;
		}
	}
	
	my $J = 0;
	while($mystring =~ m/<tr(.*?)>(.*?)<\/tr>/si) { 	# loop each <tr>
	   $mystring = $';	# continue to process the REST
	   $J++;
	   my $tmpTR = $2;	# content of this <TR>
	   
	   # check col head level and then row head layer
	   #if($tmpTR =~ /.*(<th.*?>.*?<\/th>)/si) {	# if last cell is <th> all row is <th>
	   if($tmpTR =~ /.*<\/th>(\s*)$/si) {		# if last cell is <th> all row is <th>
	   	 $N_colhead++;				# ??? what happen if this last is also the 1st cell (colspan)
	   }
	   # 1st way of checking rowhead lay: 
	   elsif ($tmpTR =~ /.*?class=.?RHlayer(\d).*/si) {	
	     if ($N_rowheadlayer < $1) { $N_rowheadlayer = $1; }
	     #print "rowheadlayer: $N_rowheadlayer\n";
	   }
	   # 2nd way of checking rowhead lay: if not col head => check row head layer
	   else {	
	   	 if($tmpTR =~ /(<th.*?>.*?<\/th>)/si) {	# check first only
	   	   $N_rowheadlayerth = 1;  	# used <th> in row head
	   	 }
	   	 elsif($tmpTR =~ /(<td.*?>.*?<\/td>)/si) {	# to be changed to consider rowspan or colspan
	   	   $N_rowheadlayertd = 1;  	# used <td> in row head
	   	 }
	   }
	   
	   # check row head level (this part needs to be improved)
	   if($InputMaxRowHeadLevel == -1) {		# for the time being, using default
	      	$N_rowhead = 1;				
	   } 	   
	}
	 
	$MaxColHeadLevel = $N_colhead;
	$MaxRowHeadLevel = $N_rowhead;
	if($N_rowheadlayer) {	# if got the info from css class RHlayer, use it
		$MaxRowHeadLayer = $N_rowheadlayer; 
	}
	else {					# otherwise, use the 2nd way: th/td
		$MaxRowHeadLayer = $N_rowheadlayerth + $N_rowheadlayertd;
	}
}
#####################################################################################
# CopyNonTableLines(*$newfile, $FirstLineNumber, $LastLineNumber, @AllLines)
#		used to copy non data table lines which need no processing
#
sub CopyNonTableLines() {
	my ($tmpNF_fh, $tmpFLN, $tmpLLN, @tmpAllLines) = @_;
	my $line;	# temp variable
				
	# copy lines from the original file (in @tmpAllLines) 
	my $tmpMM = -1;
	foreach $line (@tmpAllLines) { 
	   $tmpMM++;
	   if($tmpMM < $tmpFLN) {next;}    
	   print $tmpNF_fh "$line";
	   if($tmpMM == $tmpLLN) {last;} 	# assume no file has more than 99999 lines.
	}
}
##########################################################
# CanBeIgnored($filename, @AllLines): 
#             check if the file can be ignored:
#             empty file, index.htm(l),  all files other than html file. 
#
sub CanBeIgnored(){
	my($filename, @AllLines) = @_;
	
	if(-z $filename) {					# if file size = 0
		#print "\nfilename: ($filename) ----\n";
		if($RunMode ne "silent") {	# print if not in silent mode
		   print        " .......... Stop processing: empty file\n";
		   print $LF_fh " .......... Stop processing: empty file\n";
		}
		return 1;
	}
	if($filename =~ m/index\.htm/i) {	# if it is a index file (to skip table of content, usually called index.html)
		#print "\nfilename: ($filename) ----\n";
		if($RunMode ne "silent") {	# print if not in silent mode
		   print        " .......... Stop processing: this may be a table of content file\n";
		   print $LF_fh " .......... Stop processing: this may be a table of content file\n";
		}
		return 1;	
	}
	if($filename !~ m/.*html?$/i) {		# if it is not an html file
		#print "\nfilename: ($filename) ----\n";
		if($RunMode ne "silent") {	# print if not in silent mode
		   print        " .......... Stop processing: not an html file\n";
		   print $LF_fh " .......... Stop processing: not an html file\n";
		}
		return 1;
	} 
	return 0;		# So, this file can not be ignored.
}
##########################################################
# StartEndLineNumbers($StartLineNumber, @AllLines):  
#             find next data table (which has <th> tag) location (start & end line numbers).
#             start search from the given line number. 
#             return the start and end line numbers of the table.
#             If not found, return 99999, 99999 
sub StartEndLineNumbers() {
	my ($StartLineNumber, @AllLines) = @_;
	my $SLN = 0;		# Start Line Number
	my $ELN = 0;		# End   Line Number
	my $PossibleSLN = $StartLineNumber;
	my $InTable     = 0;	# if we are now searching in table
	my $InDataTable = 0;  	# if we are now searching in data table
	my $TmpLineNo   = $StartLineNumber;
	my ($line);
 	
  	my $tmpNN = -1; 	# a tmp. counter 
	foreach $line (@AllLines) { 		
		$tmpNN++;
		if ($tmpNN < $StartLineNumber) { next; } 	# skip previous lines
		
		# searching table head, update $PossibleSLN if found head		
		if ($line =~ m/<table/i) {	# found table head
		   if ($InTable == 0) {		# only set them when not in table
			$InTable = 1;
			$PossibleSLN = $TmpLineNo;
		   }
		}
		
		if ($InTable) {	# searching in table
			
			if ( ($TmpLineNo > $PossibleSLN) && ($line =~ m/<table/i) ) {	# found nested table
				
			 	if ($InDataTable == 1) {	# found another table head in data table
			 	   die "Error: Found nested table in data table\n"; 
			 	}
			 	else {				# found nested table in format table
			 	   $PossibleSLN = $TmpLineNo;	# reset table start line number
				}
			}
			
			if ($line =~ m/<th/i) {	 	# found data table
				if ($SLN == 0)  {
				  $SLN = $PossibleSLN;
				  $InDataTable = 1;
				}
			}
			if ( $line =~ m/<\/table>/i ) { # found table tail 
				if ($SLN == 0) {	# not found <th> yet; i.e. not in data table
				  $InTable=0; 		# => this is a format table		
				}
				else {
				  $ELN = $TmpLineNo;
				}
			}
		}
		$TmpLineNo++;
		
		if ($ELN != 0) { 		# got results already.
			return $SLN, $ELN; 	# return Start & End line numbers of the table found
		} 	 
		
	} # end of foreach
	
	return 99999, 99999;	# return 99999, 99999 to tell reached the end of file, no more data table
}
##########################################################
# Reusable($filename)
#
sub Reusable(){
	my ($filename) = @_;
	#print "In Reusable(): filename=[$filename]\n";
	if( -e $filename ) {	# if file already exist
		print "\n    File [$filename] already exist!\n";
		print   "    Do you want to reuse this file? [yes/no]: ";
		my $answer = <STDIN>;
		if($answer =~ /y/i) { return 1; }
		else 				{ return 0; }
	}
	else {					# if file does not exist
		#print "\n file does not exist!\n";
		return 1; 
	} 
}
##########################################################
# FindLocation($filename, @AllLines):  
#             find first data table (which has <th> tag)
#             then return the line number just before the table.
#
sub FindLocation() {
	my ($filename, @AllLines) = @_;
	my $LocationLine=0;		# location/line where labels will be added
	my $PossibleLocationLine=0;
	my $InTable=0;			# if we are searching in table
	my $TmpLineNo=0;
	my ($line);
	
	#print "AllLines[3]: $AllLines[3]\n";
 
	foreach $line (@AllLines) {  
		if($line =~ m/<!-- Insert Excel|CSV markers -->/) {	# already has markers?
			if($RunMode ne "silent") {	# print if not in silent mode
			   print "\nfilename: ($filename)      ";
			   print "Note: this file already has Excel|CSV markers.";
			}
			return 0;		 
		}
		# searching table head, update $PossibleLocationLine if found head
		#print "????? is table head here ....[$line]\n";
		if ($line =~ m/<table/i) {	# found table head
			$InTable=1;
			$PossibleLocationLine=($TmpLineNo-1);
			#print "##### find <table ....\n";
		}
		
		if ($InTable) {	# searching in table
			if ($line =~ m/<th/i) {	 	# found data table
				$LocationLine=$PossibleLocationLine;
				#print "----- find <th ....\n";
				#print "      [$line]\n";
				last;
			}
			if ($line =~ m/<\/table>/i) { # found table tail
				$InTable=0;
				#print "##### find </table> ....\n";
			}
		}
		$TmpLineNo++;
	} # end of foreach
	
	return $LocationLine;
}
#################################################################################
	#####################################
	#	my $string = "Walnuts are very nutritious.";
	#	my $string = '<td>12345.6</td>';
#		my $string = '<tr><Td class="33" align="right">12345.6</td></tr>';
#		print "\n\nOriginal string: [$string]\n\n";
	#	if( $string =~ /<(td.*?)>(.*?)<\/td>/i ) {
	##	if( $string =~ /<((td).*?)>(.*?)<\/\2>/i ) {
#		if( $string =~ /<((td).*?)>(.*?)<\/\2>/i ) {

		  #print "\nMatch!\n";
#		  print <<"EOF";
#		  Match!
#		  \$1 got: '$1'
#		  \$2 got: '$2'
#		  \$3 got: '$3'
#		  \$` got: "$`"
#		  \$& got: "$&"
#		  \$' got: "$'"
#EOF
#		}
#		else {
#		  print "\nNo match!\n";
#		}
		
	#####################################
#	die "testing reg...\n";

__END__
