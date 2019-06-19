<?php

class Simec_Listagem_Acao_Analise extends Simec_Listagem_Acao
{
    protected $icone = 'clipboard';
    protected $titulo = 'Análise';
//    protected $cor = 'default';


    protected function renderGlyph ()
    {
        return "<span style=\"color:#FFFFFF;\" class=\"fa fa-{$this->icone} btn btn-sm btn-primary\"></span>";
    }
}
