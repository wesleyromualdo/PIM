<?php
/**
 * $Id: Info.php 100075 2015-07-14 14:09:05Z maykelbraz $
 */

/**
 * botão listagem projovem
 */
class Simec_Listagem_Acao_Estudante extends Simec_Listagem_Acao
{
    protected $icone = 'graduation-cap';
    protected $titulo = 'Visualizar Estudante';
    protected $cor = 'info';


    protected function renderGlyph ()
    {
        return "<span class=\"btn btn-{$this->cor} btn-sm fa fa-{$this->icone}\"></span>";
    }
}
