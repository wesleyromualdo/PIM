<?php
/**
 *
 */
class Simec_Abas_Renderer_Abstract
{
    /**
     * @var
     */
    protected $config;

    public function __construct($config = null)
    {
        if ($config) {
            $this->config = $config;
        }
    }

    public function setConfig($config)
    {
        $this->config = $config;
    }
}