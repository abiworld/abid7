 <?php
 
 require('fpdf17/fpdf.php');

class PDF_Rotate extends FPDF
{
function RotatedText($x,$y,$txt,$angle)
{
        //Text rotated around its origin
        $this->Rotate($angle,$x,$y);
        $this->Text($x,$y,$txt);
        $this->Rotate(0);
}
}