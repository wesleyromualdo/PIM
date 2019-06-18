<?php
namespace MecCoder;

class View extends AbstractView
{

    use \MecCoder\View\ViewCsvTrait;
    use \MecCoder\View\ViewJsonTrait;
    use \MecCoder\View\ViewPdfTrait;
    use \MecCoder\View\ViewHtmlTrait;
}
