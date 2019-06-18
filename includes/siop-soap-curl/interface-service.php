<?php

interface SiopSoapCurl_InterfaceService {

    /**
     * Retorna o XML pra ser enviado no momento da requisição
     * 
     * @return string xml
     */
    public function loadXml();

}
