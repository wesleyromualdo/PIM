<?php
$unidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$pendencias = new Par3_Model_Pendencias();
$inuid_par2010 = $unidade->recuperarInuidPar($_REQUEST['inuid']);
$qtd_termos_nao_assinados = $pendencias->recuperaQtdTermosNaoAssinados5dias($inuid_par2010);


//verificando se a mensagem de dirigente municipal também deve ser exibida
$inuid = $_REQUEST['inuid'];

$unidadeController = new Par3_Controller_InstrumentoUnidade();
$itrid = $unidade->itrid;

//definindo perfis que verão a modal
$perfis = array(Par3_Model_UsuarioResponsabilidade::PREFEITO,
                Par3_Model_UsuarioResponsabilidade::EQUIPE_MUNICIPAL,
                Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
                Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO
        );

//instanciando modelo e verificando o perfil do usuário logado
$usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade;
$instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade;

//verificando se o usuário deve ver a modal (pelo perfil e se estiver no contexto municipal)
if($itrid == 2 && $usuarioResponsabilidade->possuiPerfil( $perfis )){
    //se o perfil atual deve ver a modal, chama a função que verifica a situação do dirigente
    $cadastroDirigenteOK = $instrumentoUnidadeEntidade->verificarDirigenteMunicipal($inuid);
}
else{
    //se o perfil do usuário não deve ver a modal, seta a flag para não mostrar
    $cadastroDirigenteOK = true;
}

include_once APPRAIZ . 'par3/classes/helper/AlertaMensagem.class.inc';

$helperAlert = new Par3_Helper_AlertaMensagem();
$perfisUsuario = is_array(pegaPerfilGeral()) ? pegaPerfilGeral() : array() ;


$naoExibeModal =
    in_array(  Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS, $perfisUsuario) ||
    in_array(Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS, $perfisUsuario);


// if (true) {
if ($qtd_termos_nao_assinados > 0 && !$naoExibeModal) {
    $btnPAR = new Par3_Helper_BotaoAcao();
    $btnPAR->add('Ir para o PAR 2011-2014!', 'redirectPAR2011', 'primary');

    $titulo = 'Pendência de validação';
    $mensagem = 'Há termo(s) de compromisso que necessita(m) validação pelo prefeito.';
    $helperAlert->addWarning($titulo, $mensagem, $btnPAR);
}

// if (true) {
if (!$cadastroDirigenteOK && !$naoExibeModal) {

    $titulo = 'Pendência de validação';
    $mensagem = 'O Dirigente Municipal de educação não está atualizado.
    É necessário atualizar o cadastro do dirigente no ícone
    <strong>Dados da Unidade</strong> no menu <strong>Dirigente municipal de educação</strong>
    para poder prosseguir com o diagnóstico do PAR.';
    $helperAlert->addWarning($titulo, $mensagem);
}

echo $helperAlert->render();

?>
<script type="text/javascript">
function redirectPAR2011() {
    location.href = '../muda_sistema.php?sisid=23';
}
</script>