<?php

require_once APPRAIZ . '/par3/classes/model/adesao_programa/Adesaoalfabetizacao.class.inc';
require_once APPRAIZ . '/includes/library/simec/Grafico.php';

$adesaoPrograma = new Par3_Model_Adesaoalfabetizacao();

switch ($_REQUEST['requisicao']) {

    case 'exportar':
        $lista = $adesaoPrograma->listaAdesoesProemiAbcMais($_REQUEST['ano']);
        header("Content-Type: application/vnd.ms-excel; charset=utf-8");
        $dataAtual = date('d-m-Y_G-i-s');
        header("Content-Disposition: attachment; filename=relatorioAdesoesAdesaoMaisAlfabetizacao_{$dataAtual}.xls");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Cache-Control: private", false);
        if (is_array($lista)) {
            ?>
            <table border="1px">
                <thead>
                <tr>
                    <th align="left">Identificação do Programa</th>
                    <th align="left">UF</th>
                    <th align="left">Código Município</th>
                    <th align="left">Município</th>
                    <th align="left">Esfera</th>

                    <th align="left">Situação do termo de adesão</th>
                    <th align="left">Situação do processo de adesão</th>

                    <th align="left">Total de escolas aptas carga horária 5h</th>
                    <th align="left">Total de escolas aptas carga horária 10h</th>
                    <th align="left">Total de escolas selecionadas carga horária 5h</th>
                    <th align="left">Total de escolas selecionadas carga horária 10h</th>
                </tr>
                </thead>
                <tbody>
                <?php foreach ($lista as $data) { ?>
                    <tr>
                        <td align="center"><?php echo $data['programa']; ?></td>
                        <td align="left"><?php echo $data['uf']; ?></td>
                        <td align="left"><?php echo $data['cod_municipio']; ?></td>
                        <td align="left"><?php echo $data['municipio_descricao']; ?></td>
                        <td align="left"><?php echo $data['esfera']; ?></td>
                        <td align="left"><?php echo $data['situacao_aceitacao_termo']; ?></td>
                        <td align="left"><?php echo $data['situacao_adesao']; ?></td>
                        <td align="left"><?php echo $data['total_escolas_5']; ?></td>
                        <td align="left"><?php echo $data['total_escolas_10']; ?></td>
                        <td align="left"><?php echo $data['total_escolas_selecionadas_5']; ?></td>
                        <td align="left"><?php echo $data['total_escolas_selecionadas_10']; ?></td>
                    </tr>
                <?php } ?>
                </tbody>
            </table>

            <?php
        }
        die;
        break;
}

require APPRAIZ . 'includes/cabecalho.inc';
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2>Relatório de Adesão - Programa Mais Alfabetização</h2>
    </div>
</div>

<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <div class="ibox">
        <div class="ibox-footer">
            <div class="row">
                <div class="col-lg-12" align="center">
                    <button type="button" class="btn btn-primary export" ano=2018><i class="fa fa-list"></i> Gerar Planilha 2018</button>
                </div>
            </div>
        </div>
    </div>
</form>

<div class="ibox">
    <div class="ibox-footer">
        <div class="row">
            <div class="col-lg-6" align="center">
                <?php
                $clsGrafico = new Grafico(Grafico::K_TIPO_PIZZA, false);
                $sql = $adesaoPrograma->getDadosGraficoUfSituacao(2018, 10, 'situacao_aceitacao_termo');
                $clsGrafico
                    ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                    ->setTitulo('Aceites do termo por UF')
                    ->gerarGrafico($sql);
                ?>
            </div>
            <div class="col-lg-6" align="center">
                <?php
                $sql = $adesaoPrograma->getDadosGraficoUfSituacao(2018, 10, 'situacao_adesao');
                $clsGrafico
                    ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                    ->setTitulo('Situação da Adesão por UF')->gerarGrafico($sql);
                ?>
            </div>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-footer">
        <div class="row">
            <div class="col-lg-6" align="center">
                <?php
                $sql = $adesaoPrograma->getDadosGraficoMunicipioSituacao(2018, 10, 'situacao_aceitacao_termo');
                $clsGrafico
                    ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                    ->setTitulo('Aceites do termo por Município')->gerarGrafico($sql);
                ?>
            </div>
            <div class="col-lg-6" align="center">
                <?php
                $sql = $adesaoPrograma->getDadosGraficoMunicipioSituacao(2018, 10, 'situacao_adesao');
                $clsGrafico
                    ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                    ->setTitulo('Situação da Adesão por Município')->gerarGrafico($sql);
                ?>
            </div>
        </div>
    </div>
</div>

<br>
<br>
<br>


<script>
    jQuery(document).ready(function () {
        jQuery('.export').click(function () {
            var ano = $(this).attr('ano');
            window.open(window.location.href + "&requisicao=exportar&ano=" + ano);
        });
    });

</script>