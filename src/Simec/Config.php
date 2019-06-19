<?php
namespace Simec;

class Config
{

    public function __construct()
    {
        
    }

    public function autoload()
    {
        spl_autoload_register(array($this, 'autoloadMecCoder'));
        spl_autoload_register(array($this, 'autoloadSimec'));
    }

    public function autoloadMecCoder($class)
    {
        $path = PHP_OS != "WINNT" ? str_replace('\\', '/', $class) : $class;
        $path = APPRAIZ . 'src/' . $path . '.php';
        if (is_file($path)) {
            include_once $path;
            return true;
        }
    }

    public function autoloadSimec($class)
    {
        if (preg_match('/Simec[\\\\](.*)/', $class, $match)) {
            $arPath = explode('\\', $match[1]);
            $arPath[0] = strtolower($arPath[0]);
            array_unshift($arPath, strtolower($arPath[0]));
            $arPath[1] = 'MecCoder';
            $class = implode('\\', $arPath);
            $path = PHP_OS != "WINNT" ? str_replace('\\', '/', $class) : $class;
            $path = APPRAIZ . $path . '.php';
            if (is_file($path)) {
                include_once $path;
                true;
            }
        }
    }
}
