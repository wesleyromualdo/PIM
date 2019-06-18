<?php
/**
 * $Id: Info.php 100075 2015-07-14 14:09:05Z maykelbraz $
 */

/**
 *
 */
class Simec_Listagem_Acao_Escola extends Simec_Listagem_Acao
{
    protected $icone = 'university';
    protected $titulo = 'Visualizar Núcleo';
    protected $cor = 'success';


    protected function renderGlyph ()
    {
        return "<span class=\"btn btn-{$this->cor} btn-sm fa fa-{$this->icone}\"></span>";
    }
}
