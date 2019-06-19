<?php
/**
 * $Id: Info.php 100075 2015-07-14 14:09:05Z maykelbraz $
 */

/**
 *
 */
class Simec_Listagem_Acao_Termo extends Simec_Listagem_Acao
{
    protected $icone = 'handshake-o';
    protected $titulo = 'Visualizar Termo';
    protected $cor = 'primary';


    protected function renderGlyph ()
    {
        return "<span class=\"btn btn-{$this->cor} btn-sm fa fa-{$this->icone}\"></span>";
    }
}
