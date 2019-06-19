<?php
/**
 * Tela de dados do prefeito
 *
 * @category visao
 * @package  A1
 * @author   Hemerson Morais
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05/07/2017
 * @link     no link
 */
$inuid = $_REQUEST['inuid'];

$sql = "SELECT estuf, muncod FROM par3.instrumentounidade WHERE inuid = {$_REQUEST['inuid']}";
global $db;
$dados = $db->pegaLinha($sql);

//instancia a classe
$oSiope = new Par3_Model_Siope();

//receitas
$receitas = $oSiope->listarReceitas($dados);
$receitas = is_array($receitas) ? $receitas : array();


?>
<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="inuid"   id="inuid"  value="<?php echo $inuid?>"/>
    <input type="hidden" name="req"                 value="salvar"/>
    <input type="hidden" name="tenid"   id="tenid"  value="<?php echo $tenid; ?>"/>
    <input type="hidden" name="menu"                value="<?php echo $_REQUEST['menu']; ?>"/>

    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-12" >
                    <h3>Receitas Vinculadas à Educação</h3>
                </div>
    		</div>
    	</div>

    	<div class="ibox-content" id="div_prefeito">
            <div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">

                <table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
                    <tr>
                        <th align="center" style="background-color: #E0E0E0;text-align:center;">
                            <b>Quanto ao Cálculo dos 25%</b>
                            <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


Conforme o Art. 212 da Constituição Federal, os Municípios deverão aplicar 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na Manutenção e Desenvolvimento do Ensino (MDE).

Segundo o Art. 70 da Lei Nº 9.394/96 (Lei de Diretrizes e Bases da Educação Nacional), MDE refere-se à consecução dos objetivos básicos das instituições educacionais de todos os níveis, compreendendo:

I - remuneração e aperfeiçoamento do pessoal docente e demais profissionais da educação;
II - aquisição, manutenção, construção e conservação de instalações e equipamentos necessários ao ensino;
III - uso e manutenção de bens e serviços vinculados ao ensino;
IV - levantamentos estatísticos, estudos e pesquisas visando precipuamente ao aprimoramento da qualidade e à expansão do ensino;
V - realização de atividades-meio necessárias ao funcionamento dos sistemas de ensino;
VI - concessão de bolsas de estudo a alunos de escolas públicas e privadas;
VII - amortização e custeio de operações de crédito destinadas a atender ao disposto nos incisos deste artigo;
VIII - aquisição de material didático-escolar e manutenção de programas de transporte escolar.">
					   <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
			    </span>
                        </th>
                        <th align="center" colspan="3" style="background-color: #B8CCE2;text-align:center">
                            <b>Receitas que entram no cálculo dos 25% de aplicação mínima em Manutenção e Desenvolvimento do Ensino (MDE)</b>
                            <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


Conforme o Art. 212 da Constituição Federal, os Municípios deverão aplicar 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na Manutenção e Desenvolvimento do Ensino (MDE).">
					<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
			</span>
                        </th>
                        <th align="center" colspan="2" style="background-color: #CADAA8;text-align:center">
                            <b>Receitas que não entram no cálculo dos 25% de aplicação mínima em Manutenção e Desenvolvimento do Ensino (MDE)</b>
                        </th>
                        <th align="center" rowspan="2" style="background-color: #E0E0E0;text-align:center">
                            <b>Total de Receitas   Vinculadas à Educação</b>
                            <br><b>(R$)</b>
                        </th>
                    </tr>
                    <tr>
                        <th align="center" style="background-color: #F5F5F5;text-align:center">
                            <b>Quais são as Receitas</b>
                        </th>
                        <td align="center" nowrap style="background-color: #EAEAEA;text-align:center">
                            <b>Originárias do FUNDEB</b>
                            <?php if($dados['muncod']){ ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU MUNICÍPIO

       O Art. 3º da Lei Nº 11.494/2007 (Lei do FUNDEB) enumera os impostos e as transferências dos quais 20% são compulsoriamente captados e direcionados para formação do FUNDEB.
       Os recursos desse fundo serão redistribuídos aos Estados e aos seus Municípios proporcionalmente ao número de alunos das diversas etapas e modalidades da educação básica presencial, matriculados nas respectivas redes, nos respectivos âmbitos de atuação prioritária estabelecidos nos §§ 2º e 3º do art. 211 da CF.
       A consequência disso é o efeito redistributivo, que provoca acréscimos ou deduções sobre a contribuição do Município ao Fundo; esses efeitos não são considerados neste campo, visto que seu produto não entra no cálculo dos 25% de aplicação mínima em MDE.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } else { ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU ESTADO

       O Art. 3º da Lei Nº 11.494/2007 (Lei do FUNDEB) enumera os impostos e as transferências dos quais 20% são compulsoriamente captados e direcionados para formação do FUNDEB.
       Os recursos desse fundo serão redistribuídos aos Estados e aos seus Municípios proporcionalmente ao número de alunos das diversas etapas e modalidades da educação básica presencial, matriculados nas respectivas redes, nos respectivos âmbitos de atuação prioritária estabelecidos nos §§ 2º e 3º do art. 211 da CF.
       A consequência disso é o efeito redistributivo, que provoca acréscimos ou deduções sobre a contribuição do estado ao Fundo; esses efeitos não são considerados neste campo, visto que seu produto não entra no cálculo dos 25% de aplicação mínima em MDE.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } ?>
                            <br><b>(R$)</b>
                        </td>
                        <td align="center" nowrap style="background-color: #EAEAEA;text-align:center">
                            <b>Impostos <?php echo $esferaTituloPlural;?> Próprios</b>
                            <?php if($dados['muncod']){ ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU MUNICÍPIO

        Conforme o Art. 212 da Constituição Federal, os municípios aplicarão anualmente 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na manutenção e desenvolvimento do ensino (MDE).
        Portanto, 25%, no mínimo, dos seguintes impostos municipais (aqueles que não entram na formação do Fundeb) deverão ser aplicados em MDE: ISS, ITBI, IPTU, Cota-Parte IOF-Ouro, Imposto de Renda Municipal.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } else { ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU ESTADO

        Conforme o Art. 212 da Constituição Federal, os estados aplicarão anualmente 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na manutenção e desenvolvimento do ensino (MDE).
        Portanto, 25%, no mínimo, dos seguintes impostos estaduais (aqueles que não entram na formação do Fundeb) deverão ser aplicados em MDE: Imposto de Renda Estadual e Cota-Parte IOF-Ouro.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } ?>
                            <br><b>(R$)</b>
                        </td>
                        <td align="center" nowrap style="background-color: #EAEAEA;text-align:center">
                            <b>Outras Receitas</b>
                            <?php if($dados['muncod']){ ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>

        Receitas de impostos e transferências computados no cálculo dos 25% de aplicação mínima em MDE, mas que não entram na formação do Fundeb, conforme no Art. 1º, I, Lei nº 11.494/2007 (Lei do FUNDEB). O <?php echo strtolower($esferaEntidade); ?>, portanto, dispõe desse recurso para aplicação em MDE, recurso esse que não integra o FUNDEB e, portanto, não está sujeito ao efeito redistributivo.">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } else { ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU ESTADO

        Receitas de impostos e transferências computados no cálculo dos 25% de aplicação mínima em MDE, mas que não entram na formação do Fundeb, conforme no Art. 1º, I, Lei nº 11.494/2007 (Lei do FUNDEB).
        O estado, portanto, dispõe desse recurso para aplicação em MDE, recurso esse que não integra o FUNDEB e, portanto, não está sujeito ao efeito redistributivo.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } ?>
                            <br><b>(R$)</b>
                        </td>
                        <td align="center" style="background-color: #D5E2BA;text-align:center">
                            <b>Salário-Educação</b>
                            <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


        Conforme o Art. 212, § 6, da Constituição Federal, o <?php echo strtolower($esferaEntidade); ?> receberá a Cota <?php echo $esferaTitulo; ?> do salário-educação, que é distribuída proporcionalmente ao número de alunos matriculados na educação básica da sua rede pública de ensino.">
    				<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;text-align:center">
    			</span>
                            <br><b>(R$)</b>
                        </td>
                        <td align="center" style="background-color: #D5E2BA;text-align:center">
                            <b>Outras Receitas destinadas à Educação</b>
                            <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


        Receitas Adicionais para Financiamento do Ensino:

        Receita da Aplicação Financeira de Outros Recursos de Impostos Vinculados ao Ensino, Transferências Diretas do Programa Dinheiro Direto na Escola (PDDE), Transferências Diretas do Programa Nacional de Alimentação Escolar (PNAE), Transferências Diretas do Programa Nacional de Apoio ao Transporte do Escolar (PNATE), Outras Transferências do FNDE, Aplicação Financeira dos Recursos do FNDE, Receitas de Transferências de Convênios, Receitas de Operação de Crédito e Outras Receitas Para o Financiamento do Ensino. ">
    					<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
    			</span>
                            <br><b>(R$)</b>
                        </td>
                    </tr>
                    <?php
                    foreach($receitas as $r) {
                        $totReceitas = $r['vl_orig_fundeb'] + $r['vl_imp_mun_proprio'] + $r['vl_outra_receita'] + $r['vl_sal_educ'] + $r['vl_outra_rec_dest_educ'];
                        ?>
                        <tr bgcolor="<?=$cor?>">
                            <td align="center">
                                <?echo $r['an_receita'] ? $r['an_receita'] : '-';?>
                            </td>
                            <td align="center">
                                <?echo $r['vl_orig_fundeb'] ? number_format($r['vl_orig_fundeb'],2,",",".") : '-';?>
                            </td>
                            <td align="center">
                                <?
                                if($dados['muncod']){
                                    echo $r['vl_imp_mun_proprio'] ? number_format($r['vl_imp_mun_proprio'],2,",",".") : '-';
                                }else{
                                    echo $r['vl_imp_est_proprio'] ? number_format($r['vl_imp_est_proprio'],2,",",".") : '-';
                                }
                                ?>
                            </td>
                            <td align="center" style="width:13%;">
                                <?echo $r['vl_outra_receita'] ? number_format($r['vl_outra_receita'],2,",",".") : '-';?>
                            </td>
                            <td align="center">
                                <?echo $r['vl_sal_educ'] ? number_format($r['vl_sal_educ'],2,",",".") : '-';?>
                            </td>
                            <td align="center">
                                <?echo $r['vl_outra_rec_dest_educ'] ? number_format($r['vl_outra_rec_dest_educ'],2,",",".") : '-';?>
                            </td>
                            <td align="center">
                                <b><?echo $totReceitas ? number_format($totReceitas,2,",",".") : '-';?></b>
                            </td>
                        </tr>
                    <?php }?>
                </table>
            </div>
        </div>

			</div>

</form>
</div>
<script>


    $(document).ready(function()
    {
        $('.next').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        $('.previous').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });
    });
</script>
