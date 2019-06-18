<?php
/**
 * Created by PhpStorm.
 * User: victor
 * Date: 07/12/17
 * Time: 13:36
 */

class SpoEmendas_Service_QuadroInformacoes {

    private $mostraDataCargaTesouro = true;
    private $mostraDataCargaSegov = true;
    private $id;

    public $height = '300';

    public function setId($id) {
        $this->id = $id;
        return $this;
    }

    public function setHeight($altura){
        $this->height = $altura;
        
        return $this;
    }
    
    public function esconderDataCargaTesouro() {
        $this->mostraDataCargaTesouro = false;
        return $this;
    }

    public function esconderDataCargaSegov() {
        $this->mostraDataCargaSegov = false;
        return $this;
    }

    public function addInformacao($mensagem, $pflcod = null) {
        $qui = new Spoemendas_Model_Quadroinformacoes();
        $qui->quimensagem = $mensagem;
        $qui->usucpf = $_SESSION['usucpf'];
        $qui->pflcod = $pflcod;
        $qui->salvar();
        if (!$qui->commit()) {
            return false;
        }
        return true;
    }

    private function montaMensagens() {
        $html = "";
        $infs = (new Spoemendas_Model_Quadroinformacoes())->getInformacoes();
        if (!empty($infs)) {
            foreach ($infs as $inf) {
                $html .= "<p class=\"p-box-message-info\"><b>{$inf['quidata']}</b> - {$inf['quimensagem']}</p>";
            }
        }
        return $html;
    }

    private function montaMensagemCargaTesouro() {
        $cargaTesouroGerencial = (new Spoemendas_Model_Tesourogerencial)->pegaDataUltimaCarga();
        $cargaTesouroGerencial = empty($cargaTesouroGerencial) ? '' : $cargaTesouroGerencial;

        $html = <<<HTML
            <p class="p-box-message-info"><b>Carga do Tesouro Gerencial:</b> {$cargaTesouroGerencial}</p>
HTML;
        return $html;
    }

    private function montaMensagemCargaSegov() {
        $cargaSegov = (new Spoemendas_Model_CargasegovHistorico())->pegaDataUltimaCarga();
        $cargaSegov = empty($cargaSegov) ? '' : $cargaSegov;

        $html = <<<HTML
            <p class="p-box-message-info"><b>Carga da SEGOV:</b> {$cargaSegov}</p>
HTML;

        return $html;
    }

    public function render($return = false) {
        $mensagens = $this->montaMensagens();
        $cargatesouro = $this->mostraDataCargaTesouro ? $this->montaMensagemCargaTesouro() : '';
        $cargasegov = $this->mostraDataCargaSegov ? $this->montaMensagemCargaSegov() : '';
        $campoid = empty($this->id) ? '' : "id=\"{$this->id}\"";
        $styleid = empty($this->id) ? ".box-mensagem-info" : "#".$this->id;

        $html = <<<HTML
            <style type="text/css">
                {$styleid} {
                    width: 100%;
                    overflow: scroll;
                    overflow-x: hidden;
                    font-size: 12px;
                    height: {$this->height}px !important;
                    /*border: 1px solid #6a6a6a;*/
                    /*background: #FFFFff;*/
                    color: #000000;mexendo
                }

                .p-box-message-info {
                    margin-bottom: 10px;
                }

                .box-mensagem-panel {
                    margin-bottom: 0px !important;
                }
            </style>

            <div class="panel panel-info box-mensagem-panel">
                <div class="panel-heading"><h3 class="panel-title">Quadro de Informações</h3></div>
                <div class="panel-body">
                    <div {$campoid} class="box-mensagem-info">
                        {$cargatesouro}
                        {$cargasegov}
                        {$mensagens}
                    </div>
                </div>
            </div>
HTML;
        if (!$return) {
            echo $html;
            return true;
        }
        return $html;
    }
}