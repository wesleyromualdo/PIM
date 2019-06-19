<?php



$controllerCoordenador = new Par3_Controller_Coordenador();

switch ($_REQUEST['requisicao']) {
    case 'salvar':
        $controllerCoordenador->salvar($_POST);
        break;
    case 'editar':
        $controllerCoordenador->editar($_POST);
        break;
    case 'download':
        $file = new FilesSimec("Assinatura Coordenador", null, false);
        $file->getDownloadArquivo($_GET['arqid']);
        break;
    case 'recuperarPorId':
        ob_clean();
        $controllerCoordenador->recuperarPorId($_REQUEST);
        die;
        break;
}

?>


<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Cadastrar Coordenador</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" method="post" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <?php $controllerCoordenador->formPesquisa(); ?>
                    </div>
                </div>

                <div class="ibox-footer">
                    <div class="center">
                        <button type="button" class="btn btn-success" id="btn-incluir" onclick="modalInserir()">
                            <i class="fa fa-plus"></i> Incluir
                        </button>
                        <button type="submit" class="btn btn-primary" id="pesquisar_coordenador" name="requisicao" value="pesquisar"><i class="fa fa-search"></i> Pesquisar
                        </button>
                        |
                        <button type="button" class="btn btn-success" id="limpar" onclick="limpaFormPesquisa()"><i class="fa fa-eraser"></i> Limpar
                        </button>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-sm-12 text-center">
                    <?php
                        $controllerCoordenador->listar($_REQUEST);
                    ?>
                </div>
            </div>
        </div>

    </div>
</div>

<!-- modal -->
<div class="ibox float-e-margins animated modal" id="modal_coordenador" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formCadastroCoordenador" id="formCadastroCoordenador" class="form form-horizontal"
              enctype="multipart/form-data">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo"></h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php
                        $controllerCoordenador->formSalvar();
                    ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="button" id="cancelarDocumentoAnexo"
                                data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="confirmarSalvar"
                                class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<script>

    $(document).ready(function () {

          $('input[type=file]').change(function () {
              var val = $(this).val().toLowerCase();
              var regex = new RegExp("(.*?)\.(jpg|jpeg|png)$");
              if(!(regex.test(val))) {
                  $(this).val('');
                  alert('Formato de arquivo não permitido. </br> Anexar arquivo somente nos seguintes formatos:  .jpg, .jpeg ou .png');
              }

              if($('#possuiAnexo').val() > 0){
                  $(this).val('');
                  alert("Permitido envio de apenas 1 arquivo.");
              }

          });

        $('#confirmarSalvar').on('click', function(){

            if($('.modal-dialog  #coonome').val() == ''){
                alert("Por favor, preencha o campo Nome.");
                return false;
            }

            if($('#possuiAnexo').val() < 1 && $('#anexo').val() == ''){
                alert("Por favor, preencha o campo Anexo.");
                $('#anexo').focus();
                return false;
            }

            $('#formCadastroCoordenador').submit();

        });

        $('#coostatus-A').change(function(e){
            console.log('afff');
            e.preventDefault();
            if($('#existeAtivo').val() > 0){
                alert('Já existe um coordenador ativo!');
                $('#coostatus-I').prop('checked','checked');
                return false;
            }

            $('#coostatus-A').prop('checked','checked');
        });

    });


    function modalInserir() {
        $('#formCadastroCoordenador').resetForm();
        $('#listaAnexo').html('');

        $('#modal_titulo').html('Incluir Coordenador');
        $('#possuiAnexo').val(0);
        $('#requisicao').val('salvar');
        $("#modal_coordenador").modal("show");
    }

    function modalEditar(id) {

        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=recuperarPorId&cooid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                dados = jQuery.parseJSON(resp);

                listarAnexo(dados.arqid, dados.arqnome, dados.arqextensao);

                $('#possuiAnexo').val(1);

                $('.modal-dialog #modal_titulo').html('Editar Coordenador');
                $('#requisicao').val('editar');

                $('.modal-dialog #cooid').val(dados.cooid);
                $('.modal-dialog #coonome').val(dados.coonome);
                $('.modal-dialog #coodtainclusao').val(dados.coodtainclusao);
                $('.modal-dialog #coobservacao').val(dados.coobservacao);
                if(dados.coostatus == 'A'){
                    $('#coostatus-A').prop('checked','checked');
                }else{
                    $('#coostatus-I').prop('checked','checked');
                }

                $("#modal_coordenador").modal("show");

            }
        });
    }

  function listarAnexo(arqid, arqnome, arqextensao) {

      var lista = "<div class='form-group' id='arquivo_" + arqid + "'>" +
          "<div class='col-md-offset-1 col-lg-offset-1 col-sm-2 control-label'></div>" +
          "<input type='hidden' name='arqid' value='" + arqid + "'>" +
          "<a class='MultiFile-remove' href='#anexo'>" +
          "<a onclick='excluirAnexo(" + arqid + ")' class='btn btn-danger btn-xs' title='Excluir'" +
          "aria-label='Close'><span aria-hidden='true'>×</span> </a> </a>   " +
          "<a title='Baixar' href='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentoAnexo&metodo=download&arqid=" + arqid + "'>" +
          "<i class='glyphicon glyphicon-cloud-download btn btn-warning btn-xs'></i></a>" +
          "<span disabled>" +
          "<span class='MultiFile-label' title='Selecionado: " + arqnome + "." + arqextensao + "'>" +
          "<span class='MultiFile-title'>" +
          "<input type='hidden' name='arquivo-selecionado' id='arquivo-selecionado' value='anterior'>  " +
          arqnome + "." + arqextensao +
          "</span>" +
          "</span>" +
          "</span>" +
          "</div>";

      $("#listaAnexo").html(lista);

  }

  /**
   * Excluir anexo
   */
  function excluirAnexo(idAnexo) {

      swal({
              title: " Remover Documento Anexo!",
              text: "Tem certeza que deseja remover o Documento Anexo?",
              showCancelButton: true,
              confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
              closeOnConfirm: "on",
              cancelButtonText: "Cancelar",
              html: true
          }
          , function (isConfirm) {
              if (isConfirm) {
                  $("#possuiAnexo").val(0);
                  $("#arquivo_" + idAnexo).html("<input type='hidden' name='idsExcluirAnexo[]' value='" + idAnexo + "'>");
              }
          });
  }



      function limpaFormPesquisa(){
          $('#cooid').val('');
          $('#coonome').val('');

          $('#pesquisar_coordenador').click();
      }

    function somenteNumeros(stringSuja){
        var valor = stringSuja.toLowerCase();
        var valor = valor.replace(/[a-zA-Z]+$|[/aáàâãéêèëíìïîãoóòöõúùüýç~´`!*+.'"!@#$%¨&*()_{}^´´.,><:;/°ºª¹²³|?\-\=\[\]]|[\b]+,/,'');
        var valor = valor.replace('  ','');
        return valor;
    }

</script>