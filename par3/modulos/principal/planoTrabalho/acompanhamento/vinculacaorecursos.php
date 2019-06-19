<?php

$sql = "SELECT estuf, muncod FROM par3.instrumentounidade WHERE inuid = {$_REQUEST['inuid']}";
global $db;
$dados = $db->pegaLinha($sql);

//instancia a classe
$oSiope = new Par3_Model_Siope();

//receitas
$receitas = $oSiope->listarReceitas($dados);
$receitas = is_array($receitas) ? $receitas : array();
//fim receitas

//despesas
$despesas = $oSiope->listarDespesas($dados);
$despesas = is_array($despesas) ? $despesas : array();
//fim despesas

//cumprimento
$cumprimento = $oSiope->listarCumprimentos($dados);
$cumprimento = is_array($cumprimento) ? $cumprimento : array();
//fim despesas'


?>
<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
    <form method="post" name="formulario" id="formulario" class="form form-horizontal">

        <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>
        <input type="hidden" name="req" value="salvar"/>
        <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>
        <input type="hidden" name="menu" value="<?php echo $_REQUEST['menu']; ?>"/>

        <div class="ibox">
            <div class="ibox-title">
                <div class="row">
                    <div class="col-md-12">
                        <h3 id="entidade">Cumprimento da Vinculação dos Recursos Destinados à Educação</h3>
                    </div>
                </div>
            </div>

            <div class="ibox-content" id="div_prefeito">
                <div class="ibox-content" id="ObrPAC" style="overflow: auto;">
                    <div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">

                        <table cellpadding="2" cellspacing="2" width="100%"
                               class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
                            <tr>
                                <td align="left" style="background-color: #E0E0E0;">
                                    <b>Vinculação</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #E0E0E0;">
                                        <b><? echo $c["an_exercicio_mde"] ? $c["an_exercicio_mde"] : '-'; ?></b>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #B8CCE2;">
                                    <b>Aplicação da receita de impostos e transferências em Manutenção e Desenvolvimento
                                        do Ensino (MDE)</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #EAEAEA;">
                                        <? echo $c['vl_apl_rec_mde'] ? number_format($c['vl_apl_rec_mde'], 2, ",", ".") : '-'; ?>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #B8CCE2;">
                                    <b>% Aplicação do Município</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #EAEAEA;">
                                        <? echo $c['vl_perc_aplic_med'] ? number_format($c['vl_perc_aplic_med'], 2, ",", ".") : '-'; ?>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #B8CCE2;">
                                    <b>% Aplicação Obrigatória</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #EAEAEA;">
                                        mínimo de 25%
                                        <span title="Conforme o Art. 212 da Constituição Federal, os Municípios deverão aplicar 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na manutenção e desenvolvimento do ensino (MDE).">
						<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
					</span>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #CADAA8;">
                                    <b>Aplicação das receitas do FUNDEB na remuneração dos profissionais do
                                        magistério</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #D5E2BA;">
                                        <? echo $c['vl_apl_rem__prof_mag_rpm'] ? number_format($c['vl_apl_rem__prof_mag_rpm'], 2, ",", ".") : '-'; ?>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #CADAA8;">
                                    <b>% Aplicação do Município</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #D5E2BA;">
                                        <? echo $c['vl_perc_aplic_mag_rpm'] ? number_format($c['vl_perc_aplic_mag_rpm'], 2, ",", ".") : '-'; ?>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #CADAA8;">
                                    <b>% Aplicação Obrigatória</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #D5E2BA;">
                                        mínimo de 60%
                                        <span title="Conforme o Art. 22 da Lei 11.949/2007 (Lei do Fundeb), pelo menos 60% dos recursos anuais totais dos Fundeb serão destinados ao pagamento da remuneração dos profissionais do magistério da educação básica em efetivo exercício na rede pública.">
						<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
					</span>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #FBC79B;">
                                    <b>Aplicação das receitas do FUNDEB em despesas com MDE, excluída a remuneração do
                                        magistério</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #FCD5B4;">
                                        <? echo $c['vl_apl_exc_rem_mag_erm'] ? number_format($c['vl_apl_exc_rem_mag_erm'], 2, ",", ".") : '-'; ?>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #FBC79B;">
                                    <b>% Aplicação do Município</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #FCD5B4;">
                                        <? echo $c['vl_perc_aplic_exc_mag_erm'] ? number_format($c['vl_perc_aplic_exc_mag_erm'], 2, ",", ".") : '-'; ?>
                                    </td>
                                <?php } ?>
                            </tr>
                            <tr>
                                <td align="left" style="background-color: #FBC79B;">
                                    <b>% Aplicação Obrigatória</b>
                                </td>
                                <?php foreach ($cumprimento as $c) { ?>
                                    <td align="center" style="background-color: #FCD5B4;">
                                        máximo de 40%
                                    </td>
                                <?php } ?>
                            </tr>
                        </table>
                    </div>
                </div>
            </div>

        </div>

    </form>
</div>
<script>


    $(document).ready(function () {
        $('.next').click(function () {
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        $('.previous').click(function () {
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });
    });
</script>
