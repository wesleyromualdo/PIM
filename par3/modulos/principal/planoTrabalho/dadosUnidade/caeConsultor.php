<?php

    if( $conselheiroEditar ){
        $objPessoaFisica = $conselheiroEditar;
    }else{
        $instrumeUndEnti = new Par3_Model_InstrumentoUnidadeEntidade();
        $objPessoaFisica = new stdClass();

        $objPessoaFisica->entcpf            = $instrumeUndEnti->entcpf;
        $objPessoaFisica->entnome           = $instrumeUndEnti->entnome;
        $objPessoaFisica->entemail          = $instrumeUndEnti->entemail;
        $objPessoaFisica->arqid             = null;
        $objPessoaFisica->cacid             = null;
        $objPessoaFisica->caeid             = null;
        $objPessoaFisica->cacfuncao         = null;
        $objPessoaFisica->cactipoocupacao   = null;
    }
?>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content animated flipInY">
            <div class="ibox-title esconde " tipo="integrantes">
                <h3>Inserir Conselheiro</h3>
            </div>
            <form method="post" name="formularioConselheiro" id="formularioConselheiro" class="form form-horizontal" enctype="multipart/form-data">
                <div class="ibox-content">
                    <input type="hidden" name="req" value="cadastrarConselheiro">
                    <input type="hidden" name="menu" value="cae">
                    <input type="hidden" name="subistituir" id="subistituir" value="">
                    <input type="hidden" name="inuid" value="<?php echo $inuid; ?>">
                    <input type="hidden" name="caeid" value="<?php echo $objCAE->caeid; ?>">
                    <input type="hidden" name="tenid" value="<?php echo Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE; ?>">
                    <?php $controllerCAE->formConselheiro($disabled, $objPessoaFisica, 'formularioConselheiro');?>
                </div>
                <div class="ibox-footer">
                    <div class="row col-md-offset-5">
                        <button type="submit" id="botao_salvar" class="btn btn-success novo" <?php echo $disabled; ?>><i class="fa fa-plus-square-o"></i> Inserir Conselheiro</button>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>

<script>
    $(document).ready(function(){
        $('#formularioConselheiro').submit(function(){

            var entcpf      = $('#formularioConselheiro').find('#entcpf-conselheiro').val();
            var inuid       = $('#formularioConselheiro').find('[name="inuid"]').val();
            var tenid       = $('#formularioConselheiro').find('[name="tenid"]').val();
            var cacfuncao   = $('#formularioConselheiro').find('[name="cacfuncao"]:checked').val();

            var controle;

            if( entcpf != '' && cacfuncao == 'V' ){
                $.ajax({
                    type: "POST",
                    url: '/par3/_ajax_cae.php',
                    data: "action=verificaVicePresidente&entcpf="+entcpf+"&cacfuncao="+cacfuncao+"&inuid="+inuid+"&tenid="+tenid,
                    async: false,
                    success: function( resp ){
                        if( resp == 'S' ){
                            var confirma = confirm("Já existe um Conselheiro com o função de Vice Presidente. Deseja realmente subistitui-lo?");
                            if( confirma ){
                                $('#substituir').val('S');
                                controle = 'S';
                            }else{
                                $('#substituir').val('N');
                                controle = 'N';
                            }
                        }
                    }
                });

                if( controle == 'N' ){
                    $('#botao_salvar').attr('disabled', false);
                    return false;
                }else{
                    $('#botao_salvar').attr('disabled', true);

                    if( !($('#entnome-conselheiro').val() != '') && $('#entcpf-conselheiro') != '' ){
                        swal('Erro', 'CPF não encontrado na Receita Federal!', 'error');
                        return false;
                    }
                }
            }else{
                $('#botao_salvar').attr('disabled', true);

                if( !($('#entnome-conselheiro').val() != '') && $('#entcpf-conselheiro') != '' ){
                    swal('Erro', 'CPF não encontrado na Receita Federal!', 'error');
                    return false;
                }
            }
        });
    });
</script>