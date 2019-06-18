<?php

require_once APPRAIZ . '/par3/classes/model/adesao_programa/Adesaoescolaacessivel.class.inc';
require_once APPRAIZ . '/includes/library/simec/Grafico.php';

$pfaid = 12;

if ($pfaid == 12) {
    $exercicio = 2018;
}

$adesaoPrograma = new Par3_Model_Adesaoescolaacessivel();
$dadosPrograma = $adesaoPrograma->recuperaDadosAdesaoPAR($exercicio);


$saldodispmun = $adesaoPrograma->getTotalSaldoDisponivel($exercicio, $pfaid, 'municipio');
$saldodispuf =  $adesaoPrograma->getTotalSaldoDisponivel($exercicio, $pfaid, 'UF');

switch ($_REQUEST['requisicao']) {

    case 'exportar':
        $lista = $adesaoPrograma->listaAdesoesEA($_REQUEST['ano']);
        header("Content-Type: application/vnd.ms-excel; charset=utf-8");
        $dataAtual = date('d-m-Y_G-i-s');
        header("Content-Disposition: attachment; filename=relatorioAdesoesAdesaoEscolaAcessivel_{$dataAtual}.xls");
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
                    </tr>
                <?php } ?>
                </tbody>
            </table>

            <?php
        }
        die;
        break;
    case 'detalhar_termo_uf':
        ob_clean();
        $listaUF = $_REQUEST['uf_termo'];
        $tipo = $_REQUEST['tipo'];
        $adesaoPrograma->detalheDirigenteTermoUF($listaUF, $tipo);
        exit();        
        break;
    case 'detalhar_termo_municipio':
        ob_clean();
        $listaMun = $_REQUEST['mun_termo'];
        $tipo = $_REQUEST['tipo'];
        $adesaoPrograma->detalheDirigenteTermoMunicipio($listaMun, $tipo);
        exit();        
        break;
}

require APPRAIZ . 'includes/cabecalho.inc';
?>

<style>
    .navbar-listagem {
        width: auto!important;
    }    
</style>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-8 col-sm-12">
        <h2>Relatório de Adesão - Programa Escola Acessível</h2>
        <ol class="breadcrumb">
            <li><a href="par3.php?modulo=inicio&acao=C">Página Inicial</a></li>
            <li><a>Relatórios</a></li>
            <li><a>Adesão de Programas</a></li>
            <li><a>Escola Acessível</a></li>
        </ol>
    </div>
    <div class="col-lg-2 col-sm-12">        
        <div class="text-center" style="border: 1px solid #ccc; margin-top: 18px;">
            <h4>Valor total do programa</h4>
            <p style="font-size: 22px;">R$ <?php echo formata_valor($dadosPrograma['pfavalortotalprograma']); ?></p>
        </div>
    </div>
    <div class="col-lg-2 col-sm-12">        
        <div class="text-center" style="border: 1px solid #ccc; margin-top: 18px;">
            <h4>Saldo disponível</h4>
            <p style="font-size: 22px;">R$ <?php echo formata_valor(($saldodispuf['saldodisp'] + $saldodispmun['saldodisp'])); ?></p>
        </div>
    </div>
</div>

<!--<form method="post" name="formulario" id="formulario" class="form form-horizontal m-t-md">
    <div class="ibox">
        <div class="ibox-footer">
            <div class="row">
                <div class="col-lg-12" align="center">
                    <button type="button" class="btn btn-primary export" ano=<?php echo $_SESSION['exercicio'] ?>><i class="fa fa-list"></i> Gerar Planilha <?php echo $_SESSION['exercicio'] ?></button>
                </div>
            </div>
        </div>
    </div>
</form>-->
<div class="row m-t-sm">
    <div class=" col-md-4">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Aceites do termo (UF)</h5>
            </div>
            <div class="ibox-content">
                <div style="height: 300px">
                    <?php
                        $clsGraficoUfTermo = new Grafico(Grafico::K_TIPO_PIZZA, false);
                        $sql = $adesaoPrograma->getDadosGraficoUfSituacao($exercicio, $pfaid, 'situacao_aceitacao_termo');
                        $clsGraficoUfTermo
                            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                            ->setPointEvent(array('click' => "detalharGraficoTermoUF('" . 'situacao_aceitacao_termo' . "',this);"))
                            ->setId('termouf')
                            ->setInnerSize(60)
                            ->gerarGrafico($sql);
                    ?>
                </div>
            </div>
        </div>
    </div>
    <div class=" col-md-4">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Situação da Adesão (UF)</h5>
            </div>
            <div class="ibox-content">
                <div style="height: 300px">
                    <?php
                        $clsGraficoUfAdesao = new Grafico(Grafico::K_TIPO_PIZZA, false);
                        $sql = $adesaoPrograma->getDadosGraficoUfSituacaoAdesao($exercicio, $pfaid);
                        $clsGraficoUfAdesao
                            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                            ->setPointEvent(array('click' => "detalharGraficoTermoUF('" . 'situacao_adesao' . "',this);"))
                            ->setId('adesaouf')
                            ->setInnerSize(60)
                            ->gerarGrafico($sql);
                    ?>
                </div>
            </div>
        </div>
    </div>
    <div class=" col-md-4">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Saldo disponível (UF)</h5>
                <span class="label label-success pull-right" style="font-size: 12px!important;">R$ <?php echo formata_valor($saldodispuf['saldodisp']); ?></span>
            </div>            
            <div class="ibox-content">
                <div style="height: 300px; overflow: auto;">
                    <?php
                        $adesaoPrograma->getDadosGraficoUFSaldoDisponivel($exercicio, $pfaid);
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class=" col-md-4">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Aceites do termo (Município)</h5>
            </div>
            <div class="ibox-content">
                <div style="height: 300px">
                    <?php
                        $clsGraficoMunicipioTermo = new Grafico(Grafico::K_TIPO_PIZZA, false);
                        $sql = $adesaoPrograma->getDadosGraficoMunicipioSituacao($exercicio, $pfaid, 'situacao_aceitacao_termo');
                        $clsGraficoMunicipioTermo
                            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                            ->setPointEvent(array('click' => "detalharGraficoTermoMunicipio('" . 'situacao_aceitacao_termo' . "',this);"))                            
                            ->setId('termomunicipio')
                            ->setInnerSize(60)
                            ->gerarGrafico($sql);
                    ?>
                </div>
            </div>
        </div>
    </div>
    <div class="col-md-4">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Situação da Adesão (Município)</h5>
            </div>
            <div class="ibox-content">
                <div style="height: 300px;">
                    <?php
                        $clsGraficoMunicipioAdesao = new Grafico(Grafico::K_TIPO_PIZZA, false);
                        $sql = $adesaoPrograma->getDadosGraficoMunicipioSituacaoAdesao($exercicio, $pfaid);
                        $clsGraficoMunicipioAdesao
                            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                            ->setPointEvent(array('click' => "detalharGraficoTermoMunicipio('" . 'situacao_adesao' . "',this);"))
                            ->setId('adesaomunicipio')
                            ->setInnerSize(60)
                            ->gerarGrafico($sql);
                    ?>
                </div>
            </div>
        </div>
    </div>
    <div class="col-md-4">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Saldo disponível (Município)</h5>
                <span class="label label-success pull-right" style="font-size: 12px!important;">R$ <?php echo formata_valor($saldodispmun['saldodisp']); ?></span>
            </div>
            <div class="ibox-content">
                <div style="height: 300px; overflow: auto;">
                    <?php
                    $adesaoPrograma->getDadosGraficoMunicipioSaldoDisponivel($exercicio, $pfaid);
                    
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>
<br>
<br>
<br>

<div id="modalDetalheUF" class="ibox float-e-margins animated modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title">
                <h5 id="titulo-modal-uf"></h5><span id="situacao-modal-uf" class="label label-primary"></span>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            </div>
            <div class="ibox-content">
                <div id="conteudo-modal-uf" style="max-height: 500px; overflow: auto"></div>
            </div>
            <div class="ibox-footer">
                <div class="form-actions col-md-offset-3 text-right">
                    <button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </div>
    </div>
</div>

<div id="modalDetalheMunicipio" class="ibox float-e-margins animated modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title">
                <h5 id="titulo-modal-municipio"></h5><span id="situacao-modal-municipio" class="label label-primary"></span>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            </div>
            <div class="ibox-content">
                <div id="conteudo-modal-municipio" style="max-height: 500px; overflow: auto"></div>
            </div>
            <div class="ibox-footer">
                <div class="form-actions col-md-offset-3 text-right">
                    <button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    jQuery(document).ready(function () {
        jQuery('.export').click(function () {
            var ano = $(this).attr('ano');
            window.open(window.location.href + "&requisicao=exportar&ano=" + ano);
        });
        
        $('.navbar-listagem').remove();
    });

    function detalharGraficoTermoUF(id, event) {
        //console.log(event.id, id, event.name);
        console.log('UF: '+JSON.parse(event.id));
        var title = '';
        var uf = JSON.parse(event.id);     
        if (id == 'situacao_aceitacao_termo') {
            title = 'Termo por UF';
        } else {
            title = 'Adesão por UF';
        }
        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=/relatorio/programa_adesao/relatorioAdesaoEscolaAcessivel&acao=I',
            data: {requisicao: 'detalhar_termo_uf', uf_termo: uf, 'tipo': id },
            async: false,
            success: function (resp) {
                //console.log(resp);
                $('#titulo-modal-uf').html(title);
                $('#situacao-modal-uf').html(event.name);
                $('#conteudo-modal-uf').html(resp);
                $('#modalDetalheUF').modal('show');
            }
        });
    }
    
    function detalharGraficoTermoMunicipio(id, event) {
        //console.log(event);
        var title = '';
        var muncod = JSON.parse(event.id);     
        if (id == 'situacao_aceitacao_termo') {
            title = 'Termo por Município';
        } else {
            title = 'Adesão por Município';
        }
        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=/relatorio/programa_adesao/relatorioAdesaoEscolaAcessivel&acao=I',
            data: {requisicao: 'detalhar_termo_municipio', mun_termo: muncod, 'tipo': id },
            async: false,
            success: function (resp) {
                //console.log(resp);
                $('#titulo-modal-municipio').html(title);
                $('#situacao-modal-municipio').html(event.name);
                $('#conteudo-modal-municipio').html(resp);
                $('#modalDetalheMunicipio').modal('show');
            }
        });
    }
    
</script>