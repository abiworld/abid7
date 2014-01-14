#!/usr/bin/perl 
#
# Purpose: 
#        This program is to be used for HTML file data table 508 processing.
#        It will first call the code cleaning program to remove unwanted tags.
#        Then, it will call the table processing program to add spcific tags for accessibility. 
# Usage: 
#        $ perl wrapper.pl SourceFileName ResultFileName
#
#        here, SourceFileName should be a html file, like the html file samed from MS Excel file.
#                             It is assumed that this file are stored in the "present" directory.
#                             It will not be changed, during this processing.
#              ResultFileName should be a html file with YYMMDDHHMMSS (date/time) label 
#                             and .html extension, like 110621153847.html 
#                             It will be created in table-processing program and will be stored in $filedir (see below).
#                             The final result will be saved in this file.
# History:
#        06/23/2011	By Jinhui Huang (Ver 1.0)
#
$version = "1.0";
######### parameter ##########
$filedir = "";


##############################
#print "This is the wrapper program ...\n";


#if(@ARGV == 2) {
#  $OldFN = $ARGV[0];				# OldFN now has SourceFileName
  #$OldFN = $filedir.$OldFN;			#       then, add ".filedir/". Use this line if it is in $filedir.
#  $NewFN = $ARGV[1];				# NewFN now has ResultFileName
#  $NewFN = $filedir.$NewFN; 			#       then, add ".filedir/"
  

  # check source file
#  if(! -r $OldFN) { print "2\n";  exit 2;}	# Error 2: Source file is not readable.
  # check result file name
#  if($NewFN !~ /^.*\/(\d*)\.html$/) {		# should be sth like, "./filedir/110621153847.html"
#     print "4-$NewFN-\n";  exit 4; 			# Error 4: Wrong result file name format. Use YYMMDDHHMMSS.html 
#  }						#          where YYMMDDHHMMSS should be numbers.
#  else {
     $TmpNewFN = $1."_tmp.html";		# Temperary result: like, 110621153847_tmp.html
#  }
#}
#else  { 		# when either one or both file names are missing
#  print "3\n"; 		# print for Drupal/php program
#  exit 3; 		# Error 3:  Missing source and/or result file name.
#}
#print "OldFN,NewFN: $OldFN, $NewFN\n";

########################### 
# run code-cleaning program
###########################
#$args = "perl prog1.pl $OldFN $NewFN";
$args = "perl codecleaner-508.pl $OldFN $TmpNewFN";
$returncode = system($args);  			# run the program with exit code
$rc = ($returncode >> 8) & 0xff;		# get the exit code
#$rc == 0 or { print "$rc\n";  exit $rc; } 	# quit if the found error in the program
if($rc != 0) { print "$rc\n";  exit $rc; }
#print "Reach the end of code-cleaning program...($rc)\n";

##############################
# run table-processing program
##############################
#$args = "perl prog2.pl $NewFN";
$args = "perl TableProcess_508.pl $TmpNewFN";
$returncode = system($args);			# run the program with exit code
$rc = ($returncode >> 8) & 0xff;		# get the exit code
if($rc != 0) { print "$rc\n";  exit $rc; }	# quit if the found error in the program

##################################
# rename file and move to $filedir
##################################
$TmpNewFN_NEW = $TmpNewFN."_NEW";
$args = "mv $TmpNewFN_NEW $NewFN";
$returncode = system($args);  

#$args = "mv $TmpNewFN* $filedir";
#$returncode = system($args); 

#########################################
# reached the end of program successfully
#########################################
#print "Reach the end of table-processing program...($rc)\n";
print "0\n"; 		# reached the end of program successfully.
exit 0;
#=======================================================================
# END NOTES:
# The Shell's $? variable contains the exit status of the Perl program.
# It can be displayed, in Linux, using command "echo $?".
#=======================================================================
