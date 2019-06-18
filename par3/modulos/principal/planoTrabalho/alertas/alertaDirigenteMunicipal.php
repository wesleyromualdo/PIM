<?php

$inuid = $_REQUEST['inuid'];

//definindo perfis que verão a modal
$perfis = array(Par3_Model_UsuarioResponsabilidade::PREFEITO, 
                Par3_Model_UsuarioResponsabilidade::EQUIPE_MUNICIPAL,
                Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
                Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO
        );

//instanciando modelo e verificando o perfil do usuário logado
$usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade;
$instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade;        

//verificando se o usuário deve ver a modal
if($usuarioResponsabilidade->possuiPerfil( $perfis )){
    //se o perfil atual deve ver a modal, chama a função que verifica a situação do dirigente
    $cadastroDirigenteOK = $instrumentoUnidadeEntidade->verificarDirigenteMunicipal($inuid);
}
else{
    //se o perfil do usuário não deve ver a modal, seta a flag para não mostrar
    $cadastroDirigenteOK = true;
}

if(!$cadastroDirigenteOK) { ?>
<script>
$(document).ready(function(){
    swal({
        title: "<span style='font-size: 27px'>Pendência de validação</span>",
        text: 'O Dirigente Municipal de eduação não está atualizado. \n\
        É necessário atualizar o cadastro do dirigente no ícone \n\
        Dados da unidade no menu Dirigente municipal de eduação \n\
        para poder prossequir com o diagnóstico do PAR.',
        //showCancelButton: true,
        //confirmButtonColor: "#DD6B55", confirmButtonText: "Ir para o PAR 2011-2014!",
        closeOnConfirm: "on",
        cancelButtonText: "Fechar",
        html: true
    });
});
</script>
<?php } ?>