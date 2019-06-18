<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 10/07/2018
 * Time: 15:37
 */

/**
 * A interação com o php está no arquivo principal: par3/modulos/principal/adesao/termos/apoio_emergencial_municipio.inc
 */
$disabled = ($this->getVisibilidadeForm() == false)?"disabled = 'disabled' " : "";

?>

<div class="panel-body">
    <!-- Formulário Orientações Gerais: -->
    <div class="row">
        <form name="form_orientacoes_gerais" method="POST" action="" id="form_orientacoes_gerais">
            <div class="row">
                <div class="col-md-12 col-md-offset-2">
                    <div class="col-md-2">
                        <p>
                            <strong>Orientações Gerais:</strong>
                        </p>
                    </div>
                    <div class="col-md-8">
                        <input type="hidden" name="requisicao" value="salva_orientacao_geral">
                        <input type="hidden" name="inuid" value="<?php echo $_GET['inuid'] ?>">
                        <input type="hidden" name="prgid" value="<?php echo $_GET['prgid'] ?>">
                        <?php echo $this->inputOrientacoesGerais(); ?>
                    </div>
                </div>
            </div>

            <div class="row">
                <div class="col-md-12 col-md-offset-6">
                    <div class="col-md-2">
                        <?php echo $this->inputButtonSalvarOrientacoesGerais(); ?>
                    </div>
                </div>
            </div>
        </form>
    </div>

    <form name="form_valor_por_escola" id="form_valor_por_escola" action="" method="POST">
    <!-- Valores sobre  -->

    <div class="row">
        <div class="col-md-12">
            <!-- Lado Direito -->
            <div class="col-md-6" style="border-right: #8daab7 solid 0.5px">
                <table class="" style="border-top: 0px none whitesmoke;">
                    <tr style="display: none;">
                        <td>
                            <?php if($this->getEstadoDocumento() == PAR3_FLUXO_FPM_REABERTO):?>
                                <input type="hidden" name="reabrir" value="S">
                            <?php endif; ?>
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align: right; padding: 20px;"><strong>Total de matrículas do Ensino Fundamental: <?php echo $this->tooltip("A"); ?></strong></td>
                        <td><input style="width: 100px;" type="text" disabled
                                   value="<?php echo $this->getTotalMatriculaEnsinoFundamental(); ?>"
                                   id="fpmvltotalmatriculaensinofundamental"></td>
                    </tr>

                    <tr>
                        <td style="text-align: right; padding: 20px;"><strong>Total de Matrículas atendidas pelo PDDE
                                Integral: <?php echo $this->tooltip("B"); ?></strong></td>
                        <td><input style="width: 100px;" type="text" disabled
                                   value="<?php echo $this->getTotalMatriculasAtendidasPddeIntegral(); ?>"
                                   id="fpmvltotalmatriculaatendidapddeintegral"></td>
                    </tr>

                    <tr>
                        <td style="text-align: right; padding: 20px;"><strong>Total de matrículas de tempo integral Censo:<?php echo $this->tooltip("C"); ?></strong></td>
                        <td><input style="width: 100px;" type="text" disabled
                                   value="<?php echo $this->getTotalMatriculaTempoIntegralCenso(); ?>"
                                   id="fpmvltotalmattempointegralcenso"></td>
                    </tr>
                    <tr>
                        <td style="text-align: right; padding: 20px;"><strong>Matrículas aptas para atendimento - critério 1: <?php echo $this->tooltip("D"); ?></strong></td>
                        <td>
                            <input style="width: 100px;" type="text" name="fpmqtmatriculaaptasatendimentocriterio1"
                                   max="<?php echo $this->getTotalMatriculasAptasAtendimentoC1(); ?>"
                                   value="<?php echo $this->getTotalMatriculasAptasAtendimentoC1(); ?>"
                                <?php echo $disabled;?>
                                   id="fpmqtmatriculaaptasatendimentocriterio1"
                                   onblur="js_validaValorCriterio1(<?php echo $this->getTotalMatriculasAptasAtendimentoC1(); ?>,this.value); js_setMenorValorMatriculasDisponiveis();">
                        </td>
                    </tr>
                </table>
            </div>

            <!-- Lado Esquerdo -->
            <div class="col-md-6">
                <table class="" style="border-top: 0px none whitesmoke;">
                    <tr>
                        <td style="text-align: right; padding: 20px;"><strong>Valor repassado pela Resolução FNDE nº
                                11/2018: <?php echo $this->tooltip("E"); ?></strong></td>
                        <td><input style="width: 100px;" type="text" disabled value="<?php echo number_format($this->getVlRepassadoResolucaoFnde2018(),2,",","."); ?>"
                                   id="fpmvlrepasseresolucaofnde2018"></td>
                    </tr>

                    <tr>
                        <td style="text-align: right; padding: 20px;"><strong id="txt_fpmvlaplicadopnmemunicipio">Valor a ser aplicado no PNME pelo município: <?php echo $this->tooltip("F"); ?></strong></td>
                        <td id="pnme_input">
                            <input style="width: 100px;"
                                <?php echo $disabled;?>
                                   maxlength="10" required="required" name="fpmvlaplicadopnmemunicipio"
                                   onblur="js_validaValorRepasseDirecionadoExecucao(this.value);"
                                   class="mascaramonetaria"
                                   type="text" value="<?php echo number_format($this->getVlAplicadoPnmeMunicipio(),2,',','.');?>" id="fpmvlaplicadopnmemunicipio">
                        </td>
                    </tr>

                    <tr>
                        <td style="text-align: right; padding: 20px;"><strong>Matrículas aptas para atendimento -
                                critério 2: <?php echo $this->tooltip("G"); ?></strong></td>
                        <td><input style="width: 100px;" type="text" disabled value="<?php echo ceil($this->getTotalMatriculasAptasAtendimentoC2());//number_format($this->getTotalMatriculasAptasAtendimentoC2(),2,".", ","); ?>"
                                   id="papemvlmataptasatendimentoc2" class="mascaradecimal"></td>
                    </tr>
                </table>
            </div>
        </div>
    </div>

    <!-- Matrículas Disponíveis -->
    <div class="row" style="padding-top: 10px;">
        <div class="col-md-8 col-md-offset-3">
            <table class="" style="border-top: 0px none whitesmoke;">
                <tr>
                    <td style="text-align: left; padding: 20px;"><strong>Total de matrículas para atendimento no PNME
                            15h: <?php echo $this->tooltip("H"); ?></strong>
                    </td>
                    <td>
                        <input style="width: 100px;" type="text" disabled value="<?php echo ceil($this->getMenorValorTotalMatriculaParaTendimento());?>" id="papemvllimitmatatendimento_menor_valor_text">
                        <input style="width: 100px;" type="hidden" disabled value="<?php echo ceil($this->getMenorValorTotalMatriculaParaTendimento());?>" id="papemvllimitmatatendimento_menor_valor">
                    </td>
                </tr>
            </table>
        </div>
    </div>

    <!-- Limite de matrículas -->
    <div class="row" style="padding-top: 10px;">
        <div class="col-md-4 col-md-offset-4">
            <div class="panel panel-primary">
                <div class="panel-heading" style="text-align: center;">
                    Matrículas a serem distribuídas nas escolas
                </div>
                <div class="panel-body">
                    <p>
                        <table class="" style="border-top: 0px none whitesmoke;">
                            <tr>
                                <td>
                    <p style="padding-left: 50%;">
                        <input type="text" disabled style="text-align: center;" value="<?php echo $this->getMenorValorTotalMatriculaParaTendimento();?>"
                               id="papemvlmatdisponiveisatendimento">
                    </p>
                    </td>
                    </tr>
                    </table>
                    </p>
                </div>
            </div>
        </div>
    </div>

    <!-- Tabela com informações das escolas do município -->
    <div class="row" style="padding-top: 10px;">
        <div class="col-md-12">
            <?php if(!empty($this->getListaEscolasDaUnidade()) && count($this->getListaEscolasDaUnidade()) > 0):?>

                <input type="hidden" name="requisicao" value="salva_por_escola">
                <div class="hidden_values">
                    <?php echo $this->getListaHiddenEscolaToSetValor(); ?>
                </div>
                <table class="table table-bordered dataTable">
                    <thead>
                        <th>Código INEP</th>
                        <th>Nome da Escola</th>
                        <th>UF</th>
                        <th>Município</th>
                        <th>Localização</th>
                        <th>Total de matrículas do 1° ao 9° ano (Censo 2017)</th>
                        <th>Matrículas PDDE Integral</th>
                        <th>Matrículas de tempo integral 1º ao 9º (Censo 2017)</th>
                        <th>Estudantes para atendimento no PNME 15h</th>
                    </thead>
                    <tbody>
                        <?php foreach($this->getListaEscolasDaUnidade() as $escola):?>
                        <tr>
                            <td><?php echo $escola['codinep']; ?></td>
                            <td><?php echo $escola['escnome']; ?></td>
                            <td><?php echo $escola['estuf']; ?></td>
                            <td><?php echo $escola['mundescricao']; ?></td>
                            <td><?php echo $escola['tp_localizacao']; ?></td>
                            <td><?php echo $escola['total_matricula_censo']; ?></td>
                            <td><?php echo $escola['total_matricula_pdde_integral']; ?></td>
                            <td><?php echo $escola['total_matricula_censo_integral']; ?></td>
                            <td>
                                <?php echo $this->inputsEscola($escola['codinep'], $escola['fpmevlestudantespnme15h'],
                                                                $escola['total_matricula_censo'],
                                                                $escola['total_matricula_pdde_integral'],$escola['total_matricula_censo_integral']); ?>
                            </td>
                        </tr>
                    <?php endforeach;?>
                    </tbody>
                </table>

            <?php endif; ?>
        </div>
    </div>
    </form>
    <!-- Botões editar e imprimir -->
    <div class="row" style="padding-top: 10px;">
        <div class="col-md-12 col-md-offset-4">
            <?php echo $this->getBtnSalvarFormulario();?>
            <?php if($this->getEstadoDocumento() == PAR3_FLUXO_FPM_EM_CADASTRAMENTO):?>
                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&requisicao=imprimir_termo_escola&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>"
                       target="_blanck">
                        <button class="btn btn-primary imprimir">Imprimir</button>
                    </a>
            <?php endif;?>
        </div>
    </div>
</div> <!-- FIM PAINEL INTEIRO-->
<div>
    <?php ?>
</div>
<!--<img src="../imagens/brasao.gif" width="45" height="45" border="0">-->
<script src="/par3/js/sistema/adesao/programa_apoio_municipio/jquery_mask.js"></script>
<script>
    $('.mascaramonetaria').mask('#.##0,00', {reverse: true});

    $(document).ready(function(){
        <?php if($this->getEstadoDocumento() == PAR3_FLUXO_FPM_EM_CADASTRAMENTO):?>
        //Se o secretário ja tiver cadastrado os valores
            <?php if(count($this->getListaEscolasDaUnidade()) > 0 ){ ?>
        js_setMenorValorMatriculasDisponiveis();
                <?php if(!empty($this->getMenorValorTotalMatriculaParaTendimento()) ||
                                $this->getMenorValorTotalMatriculaParaTendimento() == 0 ||
                                $this->getMenorValorTotalMatriculaParaTendimento() == '0'){?>

                <?php } ?>
            <?php } ?>
        <?php endif;?>

        <?php if($this->getEstadoDocumento() == PAR3_FLUXO_FPM_REABERTO):?>
        js_setMenorValorMatriculasDisponiveis();
                js_zeraValoresPorEscola();
        <?php endif;?>

        /**
         *
         * Callback para verificar se deve bloquear os valores dos campos das escolas
         * */

        $('.dataTable').dataTable({
            "fnDrawCallback": function( oSettings ) {
                if($("#papemvlmatdisponiveisatendimento").val() == 0 || $("#papemvlmatdisponiveisatendimento").val() == '0'){
                    js_bloqueiaTodosOsCamposDaEstudantesPNME15h();
                }
            },

            "language": {
                "sEmptyTable": "Nenhum registro encontrado",
                "sInfo": "Mostrando de _START_ até _END_ de _TOTAL_ registros",
                "sInfoEmpty": "Mostrando 0 até 0 de 0 registros",
                "sInfoFiltered": "(Filtrados de _MAX_ registros)",
                "sInfoPostFix": "",
                "sInfoThousands": ".",
                "sLengthMenu": "_MENU_ resultados por página",
                "sLoadingRecords": "Carregando...",
                "sProcessing": "Processando...",
                "sZeroRecords": "Nenhum registro encontrado",
                "sSearch": "Pesquisar",
                "oPaginate": {
                    "sNext": "Próximo",
                    "sPrevious": "Anterior",
                    "sFirst": "Primeiro",
                    "sLast": "Último"
                },
                "oAria": {
                    "sSortAscending": ": Ordenar colunas de forma ascendente",
                    "sSortDescending": ": Ordenar colunas de forma descendente"
                }
            }
        });

    });

    /**
     * Mascara monetária
     */
    $('#fpmvlaplicadopnmemunicipio').on('paste keyup blur', function () {
        //this.value = this.value.split(/[^\d,]/).filter(Boolean).join('');
        var valuePNME = this.value;
        var parts = this.value.split(',').filter(Boolean);
        if (parts.length > 1) this.value = [parts.shift()].concat(parts.join('')).join(',');
        js_atualizaCriterio2(valuePNME);
        js_setMenorValorMatriculasDisponiveis();
    });


    function js_restartaValorPorEscola(){
        $(".vl_escola").val(0);
        $(".vl_escola").trigger("blur");
        $(".vl_escola").removeAttr("disabled");
    }


    /**
     *
     **/
    function js_atualizaCriterio2(valuePNME){
        var valorFNDE = $("#fpmvlrepasseresolucaofnde2018").val();
        setaTotalMatriculasAtendimentoPNME15h();
    }

    /**
     *  Seta o menor valor em Total de matrículas para atendimento no PNME 15h: e  Matrículas a serem distribuídas nas escolas
     *
     **/
    function setaTotalMatriculasAtendimentoPNME15h(){
        var c1 = $("#fpmqtmatriculaaptasatendimentocriterio1").val();
        var c2 = $("#papemvlmataptasatendimentoc2").val();

        c2Inicial = c2;

        c1 = (c1 == "") ? 0 : c1;
        c1 = parseInt(c1);

        c2 = c2.replace(".", "");
        c2 = c2.replace(",", ".");

        c2 = parseFloat(c2);
        //c2_arredondado = Math.ceil(c2);
        if(c1 > c2){
            $("#papemvllimitmatatendimento_menor_valor_text").val(c2Inicial);
            $("#papemvllimitmatatendimento_menor_valor").val(Math.ceil(c2));
            $("#papemvlmatdisponiveisatendimento").val(Math.ceil(c2));
            $("#papemvlmataptasatendimentoc2").val(parseInt(Math.ceil(c2)));
        }else{
            $("#papemvllimitmatatendimento_menor_valor_text").val(c1);
            $("#papemvlmatdisponiveisatendimento").val(c1);
        }
    }



    /**
     * Mascara somente números inteiros
     */
    $('#fpmqtmatriculaaptasatendimentocriterio1').on('paste keyup', function () {
        value = this.value;
        if(value != 0 || value != '0'){
            this.value  = value.replace(/[^\d]+/g,'');
        }
    });

    $('#fpmqtmatriculaaptasatendimentocriterio1').on('blur', function () {
        value = this.value;
        if(value != 0 || value != '0'){
            this.value  = value.replace(/[^\d]+/g,'');
        }
        js_setMenorValorMatriculasDisponiveis();
    });

    /***
     * Apresenta nos campos Total de matrículas para atendimento no PNME 15h: e  Matrículas a serem distribuídas nas escolas
     * o valor menor entre Critério 1 e 2
     * */
    function js_setMenorValorMatriculasDisponiveis(){
        js_restartaValorPorEscola();

        var c1 = $("#fpmqtmatriculaaptasatendimentocriterio1").val();
        var c2 = $("#papemvlmataptasatendimentoc2").val();
        c2_inicial = c2;
        c1 = (c1 == "") ? 0 : c1;
        c1 = parseInt(c1);

        c2 = c2.replace(",", ".");

        c1 = parseFloat(c1);

        if(c1 > c2){
            //c2 = Math.round(c2);
            $("#papemvllimitmatatendimento_menor_valor_text").val(c2_inicial);
            $("#papemvllimitmatatendimento_menor_valor").val(Math.ceil(c2));
            $("#papemvlmatdisponiveisatendimento").val(Math.ceil(c2));
            $("#papemvllimitmatatendimento_menor_valor_text").val(Math.ceil(c2));
            $("#papemvlmataptasatendimentoc2").val(parseInt(Math.ceil(c2)));
        }else{
            $("#papemvllimitmatatendimento_menor_valor_text").val(c1);
            $("#papemvlmatdisponiveisatendimento").val(c1);
            $("#papemvllimitmatatendimento_menor_valor").val(c1);
        }
    }



    /***
     * Apresenta nos campos Total de matrículas para atendimento no PNME 15h: e  Matrículas a serem distribuídas nas escolas
     * o valor menor entre Critério 1 e 2
     * */
    function js_setMenorValorMatriculasDisponiveisFluxoCadastramento(){
        var c1 = $("#fpmqtmatriculaaptasatendimentocriterio1").val();
        var c2 = $("#papemvlmataptasatendimentoc2").val();
        c2_inicial = c2;
        c1 = (c1 == "") ? 0 : c1;
        c1 = parseInt(c1);

        c2 = c2.replace(",", ".");

        c1 = parseFloat(c1);

        var mostraZeradoMatriculasASeremDistribuidas = false;
        var totalJaCadastrada = js_getSomaTotalHiddenEscola();
        if(parseInt(totalJaCadastrada) > 0){
            mostraZeradoMatriculasASeremDistribuidas = true;
        }

        if(c1 > c2){
            //c2 = Math.ceil(c2);
            $("#papemvllimitmatatendimento_menor_valor_text").val(c2_inicial);
            $("#papemvllimitmatatendimento_menor_valor").val(Math.ceil(c2));
            if(true == mostraZeradoMatriculasASeremDistribuidas){
                $("#papemvlmatdisponiveisatendimento").val(0);
            }else{
                $("#papemvlmatdisponiveisatendimento").val(Math.ceil(c2));
            }

        }else{
            $("#papemvllimitmatatendimento_menor_valor_text").val(c1);
            if(true == mostraZeradoMatriculasASeremDistribuidas){
                $("#papemvlmatdisponiveisatendimento").val(0);
            }else{
                $("#papemvlmatdisponiveisatendimento").val(c1);
            }
            $("#papemvlmatdisponiveisatendimento").val(c1);
        }
    }

/*    $(".mascaramonetaria").on('paste keyup blur', function () {
        this.value = this.value.split(/[^\d,]/).filter(Boolean).join('');
        var parts = this.value.split(',').filter(Boolean);
        if (parts.length > 1) this.value = [parts.shift()].concat(parts.join('')).join(',');
    });*/


    function js_editarValorTabelaEscolasTemCerteza(){
        swal({
            title: "<span style='font-size: 27px'>Atenção</span>",
            text: "O plano do município que for reaberto para edição deverá ser reenviado até a data final da adesão. Deseja continuar ?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
               js_tramitaReabrir();
            } else {
                return false;
            }
        });
    }

    //onclick no botão editar
    function js_editarValorTabelaEscolas() {
        var formValues = $(".vlestudanteporescola").val();
        var matriculasDistribuidasNasEscolas = parseInt($("#papemvlmatdisponiveisatendimento").val());
        var fpmvlaplicadopnmemunicipio = $("#fpmvlaplicadopnmemunicipio").val();
        var vl_fpmvlaplicadopnmemunicipio = fpmvlaplicadopnmemunicipio.replace(".","");
        vl_fpmvlaplicadopnmemunicipio = vl_fpmvlaplicadopnmemunicipio.replace(",",".");
        vl_fpmvlaplicadopnmemunicipio = parseFloat(vl_fpmvlaplicadopnmemunicipio);
        //console.log(vl_fpmvlaplicadopnmemunicipio);
        if(vl_fpmvlaplicadopnmemunicipio < 120){
            js_msg("O valor mínimo deve ser R$ 120,00, correspondente ao custo médio de um estudante para o Novo Mais Educação para o período de 4 meses");
            return false;
        }

        if(matriculasDistribuidasNasEscolas == 0){
            $("#form_valor_por_escola").submit();
        }else{
            js_msg("As  Matrículas a serem distribuídas nas escolas devem ser totalamente utilizadas!");
            return false;
        }
    }

    //onclick no botão imprimir
    function js_imprimirValorTabelaEscola() {
        var inuid = '<?php echo $_GET['inuid'];?>';
        url = window.location.href;
        url = url.replace("#tab-2","");
        url = url.replace("#tab-1","");
        url = url+"&requisicao=imprimir_plano_unidade";
        window.location = url;
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param value
     * @description O sistema não deve permitir o preenchimento de valor menor que R$120,00 e
     * maior que o "Valor repassado pela Resolução FNDE nº 11/2018".
     *  Se for preenchido valor inferior a R$ 120,00, abrir pop-up informando ?O valor mínimo deve ser R$ 120,00, correspondente ao custo médio
     *  de um estudante para o Novo Mais Educação para o período de 4 meses.? .
     */
    function js_validaValorRepasseDirecionadoExecucao(value) {
        var vlInformado = value.replace(".", "");
        vlInformado = vlInformado.replace(",", ".");

        var vlValorRepassadoResolucaoFNDE112018 = $("#fpmvlrepasseresolucaofnde2018").val();
        vlValorRepassadoResolucaoFNDE112018 = vlValorRepassadoResolucaoFNDE112018.replace(".", "");
        vlValorRepassadoResolucaoFNDE112018 = vlValorRepassadoResolucaoFNDE112018.replace(",", ".");
        vlValorRepassadoResolucaoFNDE112018 = parseFloat(vlValorRepassadoResolucaoFNDE112018);
        console.log(vlValorRepassadoResolucaoFNDE112018);

        var vlMinimo = 120.00;

        /**
         * O valor mínimo deve ser 120.00
         */
        if (vlInformado < vlMinimo) {
            js_msg("O valor mínimo deve ser R$ 120,00, correspondente ao custo médio de um estudante para o Novo Mais Educação para o período de 4 meses.");
            //$("#fpmvlaplicadopnmemunicipio").val("120,00");
           // $("#txt_fpmvlaplicadopnmemunicipio").css("color", "red");

        }


        /**
         *O sistema não deve permitir o preenchimento de valor menor que R$120,00 e maior que o "Valor repassado pela Resolução FNDE           nº 11/2018". Se for preenchido valor inferior a R$ 120,00, abrir pop-up informando ?O valor mínimo deve ser R$ 120,00,               correspondente ao custo médio de um estudante para o Novo Mais Educação para o período de 4 meses
         */

        if (vlInformado > vlValorRepassadoResolucaoFNDE112018) {
            js_msg("O valor do campo 'Valor a ser aplicado no PNME pelo município' não pode ser superior ao valor do campo 'Valor repassado pela Resolução FNDE nº 11/2018'");
            //$("#fpmvlaplicadopnmemunicipio").val("120,00");
           // $("#txt_fpmvlaplicadopnmemunicipio").css("color", "red");
            $("#fpmvlaplicadopnmemunicipio").val(0);
        }

       // var vl_matriculas_criterio2 = (vlValorRepassadoResolucaoFNDE112018 / vlInformado);
        var vl_matriculas_criterio2 = (vlInformado / 120);
        vl_matriculas_criterio2 = (Math.ceil(vl_matriculas_criterio2));
        $("#papemvlmataptasatendimentoc2").val(vl_matriculas_criterio2);
        setaTotalMatriculasAtendimentoPNME15h();
        js_restartaValorPorEscola();
        js_setMenorValorMatriculasDisponiveis();

    }

    /**
     *
     * Formata valor numérico
     * */
    function formataParaReal(numero) {
        //console.log(numero);
        if((numero == Infinity)){
            return 0;
        }else{
            var numero = numero.toFixed(3).split('.');
            numero[0] = numero[0].split(/(?=(?:...)*$)/).join('.');
            return numero.join(',');
        }

    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param deveSalvar
     * @description Este método verifica se o usuário quer alterar o valor do campo Orientações Gerais
     */
    function js_salvaValorOrientacoesGerais(deveSalvar) {
        if (deveSalvar == true) {
            var goSubmit = false;
            var pfasobreHTML = $('[name=pfasobre]').code().trim(); //pega o valor do textarea
            var pfasobre = pfasobreHTML.replace(/<(.|\n)*?>/g, ''); //remove todo html pra ver se existe algum texto
            var prgid = $('[name=prgid]').val(); //pega o valor do id do programa para utilizar no update
            //alert(prgid);
            if (pfasobre != "" && prgid != "") {
                swal({
                    title: "<span style='font-size: 27px'>Atenção</span>",
                    text: "Você deseja alterar as Orientações Gerais ? ",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                    closeOnConfirm: "on",
                    cancelButtonText: "Cancelar",
                    html: true
                }, function (isConfirm) {
                    if (isConfirm) {
                        $("#form_orientacoes_gerais").submit();
                    } else {
                        return false;
                    }
                });
            } else {
                swal({
                    title: "Atenção!",
                    text: "O campo Orientações Gerais não pode ser vazio!",
                    icon: "error",
                    buttons: ["ok!", true],
                });
                return false;
            }

        }
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param text
     * @description Esta função emite um alert usando o plugin swal
     */
    function js_msg(text) {
        swal({
            title: "Atenção!",
            text: text,
            icon: "error",
            buttons: ["ok!", true],
        });
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param id_hidden
     * @param valor_digitado
     */
    function js_setValueHidden(id_hidden, valor_digitado) {

        idHidden = 'vl_' + id_hidden;
       // console.log(idHidden);
        $("#" + idHidden).val(id_hidden + "|" + valor_digitado);
    }

    $(".sonumero").on('paste keyup', function () {
        value = this.value;
        if(value != 0 || value != '0'){
            this.value  = value.replace(/[^\d]+/g,'');
        }
    });


    /**
     * @param maxValue
     * @param value
     */
    function js_validaValorCriterio1(maxValue,value){
        if(value == "" || value == 0){
            $("#fpmqtmatriculaaptasatendimentocriterio1").val(maxValue);
        }
        value = (value == "") ? 1 : value;
        maxValue = (maxValue == "") ? 0 : maxValue;
        if(maxValue < value || value == ""){
            js_msg("O valor informado neste campo não pode ser superior ao total de matrículas aptas para atendimento");
            $("#fpmqtmatriculaaptasatendimentocriterio1").val(maxValue);
        }
        setaTotalMatriculasAtendimentoPNME15h();
        js_setMenorValorMatriculasDisponiveis();
    }

    /**
     *
     * @param id
     * @param value
     */
    function js_validaValorPorEscola(id,value,a,b,c){
        var c = ((a-b) - c);
        var c1 = $("#fpmqtmatriculaaptasatendimentocriterio1").val();
        c1 = (c1 == "") ? 0 : c1;
        c1 = parseInt(c1);
        value = parseInt(value);

       if(value > c){
            js_msg("O valor  de de estudantes não pode passar o valor da escola de: " + c);
            $("#"+id).val(c);
            $("#vl_"+id).val(id+"|"+c);

        }

        //caso valor seja válido já seta no hidden
        if(value <= c){
            $("#vl_"+id).val(id+"|"+value);
        }

        js_subtraiMatriculasDisponiveis();
    }

    /**
     * retorna a somatória dos valores da escola no hidden
     * */
    function js_getSomaTotalHiddenEscola(){
        var url =  window.location.href;
        var unidade = "<?php echo $_GET['inuid'];?>";
        var requisicao = "valida_soma_total_por_escola";
        var valorHidden = $(".vlestudanteporescola").val();
        var valueTotal = 0;
        $(".vlestudanteporescola").each(function( index ) {
            arrayDeValores = this.value.split("|");
            valueTotal = (valueTotal + parseInt(arrayDeValores[1]));
        });
        return valueTotal;
    }

    /**
     * Bloqueia todas os campos de preenchimento da tabela
     * */
    function js_bloqueiaTodosOsCamposDaEstudantesPNME15h(){
        $(".vl_escola").each(function( index ) {
            $(this).attr("disabled","disabled");
        });
        //$(".pagination").html("");
        //$(".dataTable").attr("disabled","disabled");
    }





    function js_subtraiMatriculasDisponiveis(){
        var vl_maximo = $("#papemvllimitmatatendimento_menor_valor").val();
        var vl_escolas = js_getValuesPorEscolaDigitados();
        var subtracao = 0;
        if(vl_escolas > vl_maximo){
            js_msg("A soma dos Estudantes para atendimento no PNME 15h não pode ser maior que a Matrículas a serem distribuídas nas escolas !");
            return false;
        }

        vl_escolas = (vl_escolas == "") ? 0 : vl_escolas;
        if(vl_escolas == 0){
            subtracao = vl_maximo;
        }else{
            subtracao = (vl_maximo - vl_escolas)
        }

        if(subtracao == 0 || subtracao == "0"){
            js_bloqueiaTodosOsCamposDaEstudantesPNME15h();
        }

        $("#papemvlmatdisponiveisatendimento").val(subtracao);
    }

    /**
     * SOMA TODOS OS VALORES DIGITADOS EM: Estudantes para atendimento no PNME 15h
     **/
    function js_getValuesPorEscolaDigitados(){
        return js_getSomaTotalHiddenEscola();
    }


    /*$(document).ready(function(){*/
        setaTotalMatriculasAtendimentoPNME15h();
    /*})*/

    /**
     *
     * @param dataLimit
     */
    function js_reabrirPlano(dataLimit){
        swal({
            title: "<span style='font-size: 27px'>Atenção</span>",
            text: "O plano do município que for reaberto para edição deverá ser reenviado até a data final da adesão. Deseja continuar? ",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                js_tramitaReabrir();
            } else {
                return false;
            }
        });
    }

    function js_tramitaReabrir(){
        var url =  window.location.href;
        var unidade = "<?php echo $_GET['inuid'];?>";
        var docid   = "<?php echo $this->getDocId(); ?>";
        var requisicao = "tramita_reabrir";
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: "&inuid="+unidade+"&docid="+docid+"&requisicao="+requisicao,
            success: function(returno) {
               if(returno == "T"){
                   uri = url.split("#");
                   if(uri[0]){
                       window.location = uri[0]
                   }else{
                       window.location = window.location.href;
                   }
               }
            }
        });
    }

    function js_tramitaEnviarMEC(){
        swal({
            title: "<span style='font-size: 27px'>Atenção</span>",
            text: "Você deseja realmente enviar sua adesão ao FNDE ?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                var url =  window.location.href;
                var unidade = "<?php echo $_GET['inuid'];?>";
                var docid   = "<?php echo $this->getDocId(); ?>";
                var requisicao = "tramita_enviadofnde";
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: "&inuid="+unidade+"&docid="+docid+"&requisicao="+requisicao,
                    success: function(returno) {
                        if(returno == "T"){
                            uri = url.split("#");
                            if(uri[0]){
                                window.location = uri[0]
                            }else{
                                window.location = window.location.href;
                            }
                        }
                    }
                });
            } else {
                return false;
            }
        });

    }

    function js_zeraValoresPorEscola(){
        $(".vl_escola").val("0");
    }

    function formatBR(value, decimais) {

        decimais = decimais || 2;
        var mi = value.length - parseInt(decimais);
        var sm = parseInt(mi / 3);
        var regx = "", repl = "";

        for (var i = 0; i < sm; i++) {
            regx = regx.concat('([0-9]{3})');
            repl = repl.concat('.$' + (i + 1));
        }

        regx = regx.concat('([0-9]{' + decimais + '})') + "$";
        repl = repl.concat(',$' + (sm + 1));
        value = value.toString().replace(new RegExp(regx, 'g'), repl);
        return (mi % 3) === 0 ? value.substr(1) : value;
    }


</script>