<?php
/**
 * Alterações gerais da emenda.
 *
 * @version $Id: geral.inc 108692 2016-03-01 14:22:46Z maykelbraz $
 */

$dadosAutor = (new Spoemendas_Model_Autor())
    ->pegaPorEmeid($_REQUEST['emeid']);

$htmlPanel = <<<DML
<table class="table table-condensed table-striped">
    <tbody>
        <tr>
            <td class="text-right"><strong>Nome:</strong></td>
            <td>{$dadosAutor['autnome']}
            <td class="text-right"><strong>E-mail:</strong></td>
            <td>{$dadosAutor['autemail']}
        </tr>
        <tr>
            <td class="text-right"><strong>Tipo de autor:</strong></td>
            <td>{$dadosAutor['tpanome']}</td>
            <td class="text-right"><strong>Grupo:</strong></td>
            <td>{$dadosAutor['gpanome']}</td>
        </tr>
        <tr>
            <td class="text-right"><strong>Categoria:</strong></td>
            <td>{$dadosAutor['gpacategoria']}</td>
            <td class="text-right"><strong>&nbsp;</strong></td>
            <td>&nbsp;</td>
        </tr>
    </tbody>
</table>
DML;

bootstrapPanel('Dados do autor', $htmlPanel);

(new Simec_View_Form('geral'))
    ->carregarDados($dadosAutor)
    ->addHidden('autid', $dadosAutor['autid'])
    ->addCombo('Partido', 'parid', Spoemendas_Model_Partido::getQueryCombo())
    ->setRequisicao('alterarpartido')
    ->addBotao('salvar')
    ->render();
