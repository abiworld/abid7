<?php
define('FPDF_FONTPATH','font/');

require_once('fpdi/rotation.php');
set_magic_quotes_runtime(0);


    class PDF extends PDF_Rotate
    {
        function Header()
        {
            //Put the watermark
            //$this->SetFont('Arial','B',50);
            $this->SetTextColor(255,192,203);
            $this->RotatedText(35,190,'Preview Only!',45);
        }

        function RotatedText($x, $y, $txt, $angle)
        {
            //Text rotated around its origin
            $this->Rotate($angle,$x,$y);
            $this->Text($x,$y,$txt);
            $this->Rotate(0);
        }
    }