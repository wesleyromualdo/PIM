<?php
/**
 * Classe de controle do analise_engenharia_obra_aba_perguntas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   Daniel Fiuza <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-10-2017
 * @link     no link
 */


/**
 * Par3_Controller_AnaliseEngenhariaObraAbaPerguntas
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-10-2017
 * @link     no link
 */
class Par3_Controller_AnaliseEngenhariaObraAbaPerguntas extends Controle
{
    private $model;

    public function __construct()
    {
      //  $this->model = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas();
    }

    public function carregarPergunta($oabid, $aepordem, $obrid = null)
    {
        if($obrid == null){
            return false;
        }
//        descomentar para testar com a tramitação
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        $pflid = pegaPerfilGeral($_SESSION['usucpf']);
        $mAnalise  =  new Par3_Model_AnaliseEngenhariaObra();

        //VALIDA SE O ESTADO DOCUMENTO ATUAL DEVE MOSTRAR A ANÁLISE
        if(
            (!array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao()))   ||
            (!array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadoValidacao()))
        ){
            return false;
        }

        $permiteEditar = false;
        if((array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao()))||
            (array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadoValidacao()))){
            $permiteEditar = true;
        }

        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();

        $mAep     = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas($oabid, $aepordem);

        $estadosHabilitados = [
            Par3_Model_Obra::ESDID_EM_DILIGENCIA,
            Par3_Model_Obra::ESDID_VALIDACAO_DILIGENCIA,
        ];

        if(in_array($esd['esdid'],$mAnalise->estadosCorrecaoAnalise()) || in_array($esd['esdid'],$mAnalise->estadoValidacao())){
            $aeg = $mAnalise->pegarUltimaAnalise($obrid);
            $aegid = $aeg['aegid'];
        }
        if(array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao())) {
            $aegid = $mAnalise->iniciarAnalise($obrid);
        }
        if(!$aegid){
            return false;
        }

        $mAer = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aegid,$mAep->aepid);

        //Recupera resposta da última análise
        if($mAer->aer_observacao == null) {
            $aegidUltimaAnalise = $mAnalise->pegarUltimaAnalise($obrid)['aegid'];
            $mAerUltimaAnalise = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aegidUltimaAnalise,$mAep->aepid);

            $mAer->aer_observacao = $mAerUltimaAnalise->aer_observacao;
        }

        //Não mostra caixa de pendencia para uma pergunta sem resposta para usuários que não podem editar
        if(!array_intersect($pflid,$mAnalise->perfisEdicao()) && !$mAer->aerid){
            return false;
        }

        if(!$mAer->aer_pendencia && !in_array($esd['esdid'],$mAnalise->estadosEdicao())) {
           return true;
        }

        if($oabid == Par3_Model_ObraAba::ESTUDO_DEMANDA) {
            $mEstudoDemandaObs = new Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacao();
            $observacao = $mEstudoDemandaObs->recuperarObservacao($aegid);
            $mAer->aer_observacao = $observacao['aed_observacao'];
        }
        require APPRAIZ . 'par3/modulos/principal/planoTrabalho/obras/analise_engenharia/template_pergunta.inc';
    }


    public function carregarPerguntaEstudoDemanda($oabid, $aepordem,$octid = null, $obrid = null)
    {
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        if($obrid == null && $octid == null){
            return false;
        }

        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        $perfis = [Par3_Model_UsuarioResponsabilidade::PREFEITO ,Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL];
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);

        //VALIDA SE O ESTADO DOCUMENTO ATUAL DEVE MOSTRAR A ANÁLISE
        if(
            (!array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao()))   ||
            (!array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadoValidacao()))
        ){
            return false;
        }

        $permiteEditar = false;
        if((array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao()))||
            (array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadoValidacao()))){
            $permiteEditar = true;
        }

        $mAep     = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas($oabid, $aepordem);


        if(in_array($esd['esdid'],$mAnalise->estadosCorrecaoAnalise()) || in_array($esd['esdid'],$mAnalise->estadoValidacao())){
            $aeg = $mAnalise->pegarUltimaAnalise($obrid);
            $aegid = $aeg['aegid'];
        }

        if(array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao())) {
            $aegid = $mAnalise->iniciarAnalise($obrid);
        }
        if(!$aegid){
            return false;
        }

        $mAer = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aegid,$mAep->aepid);

        //Não mostra caixa de pendencia para uma pergunta sem resposta para usuários que não podem editar
        if(!array_intersect($pflid,$mAnalise->perfisEdicao()) && !$mAer->aerid){
            return false;
        }

        if($mAer->aer_pendencia && $mAer->aer_corrigido){
//            return true;
        }

        $mEstudoDemandaObs = new Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacao();
        $observacao = $mEstudoDemandaObs->recuperarObservacao($aegid);
        $mAer->aer_observacao = $observacao['aed_observacao'];

        //Recupera resposta da última análise
        if($mAer->aer_observacao == null) {
            $aegidUltimaAnalise = $mAnalise->pegarUltimaAnalise($obrid)['aegid'];
            $observacao = $mEstudoDemandaObs->recuperarObservacao($aegidUltimaAnalise);
            $mAer->aer_observacao = $observacao['aed_observacao'];
        }

        if($octid == 1 && $aepordem == 1){$mAep->aepdsc = 'O motivo da Construção está coerente com a proposta apresentada?';}
        if($octid == 2 && $aepordem == 1){$mAep->aepdsc = 'O motivo da Ampliação está coerente com a proposta apresentada?';}
        if($octid == 1 && $aepordem == 6){$mAep->aepdsc = 'O proponente demonstrou por meio da justificativa a necessidade de construção e a quantidade mínima?';}
        if($octid == 2 && $aepordem == 6){$mAep->aepdsc = 'O proponente demonstrou por meio da justificativa a necessidade de ampliação e a quantidade mínima?';}

        require APPRAIZ . 'par3/modulos/principal/planoTrabalho/obras/analise_engenharia/template_pergunta_estudo_demanda.inc';
    }

    public function verificarAnalisePendencia($oabid, $aepordem, $obrid)
    {
        if(!$obrid){
            return false;
        }
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->mostrarPendenciasEmAnalise($obrid)){
            return false;
        }
        $analise = $mAnalise->recuperarAnalisePorObra($obrid);
        $mAep = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas($oabid, $aepordem);
        $mAer = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($analise['aegid'],$mAep->aepid);
        if(!$mAer){
            return false;
        }
        return $mAer->aer_pendencia == 'S'? true:false;
    }

    public function verificarAnaliseCorrigida($oabid, $aepordem, $obrid)
    {
        if(!$obrid){
            return false;
        }
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->mostrarPendenciasEmAnalise($obrid)) {
            return false;
        }
        $analise = $mAnalise->recuperarAnalisePorObra($obrid);
        $mAep = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas($oabid, $aepordem);
        $mAer = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($analise['aegid'],$mAep->aepid);
        if(!$mAer){
            return false;
        }
        return $mAer->aer_corrigido ? true:false;
    }

    function habilitarCamposEdicaoPorAbaOrdem($oabid,$ordem,$obrid)
    {
        $pflcod   = pegaPerfilGeral($_SESSION['usucpf']);
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();

        if(array_intersect($pflcod,$mAnalise->perfisSuperUsuario())) {
            return '';
        }

        $mObra                 = new Par3_Model_Obra($obrid);
        $mpergunta             = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        $pendenciaAnalise       = $mpergunta->verificarAnalisePendencia($oabid,$ordem,$obrid);
        $pendenciaPreenchimento = $mAnalise->verificarExistePendencia($obrid,$oabid);
        $pendenciaDadosBasicos  = $pendenciaAnalise || $pendenciaPreenchimento ? 'S':'';

        //validação para habilitar abas caso haja pendência na primeira aba do estudo de demanda
        if($oabid == Par3_Model_ObraAba::ESTUDO_DEMANDA) {
            $arrSituacoesHabilitadas = [
                $mObra::ESDID_EM_DILIGENCIA,
                $mObra::ESDID_EM_CADASTRAMENTO_CONDICIONAL,
            ];
            if(
                array_intersect($pflcod,$mAnalise->perfisCorrecaoAnalise()) &&
                in_array($esd['esdid'],$arrSituacoesHabilitadas)
            ){
                $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
                if($mAnalise->verificarExistePendencia($obrid,Par3_Model_ObraAba::ESTUDO_DEMANDA)) {
                    $pendenciaDadosBasicos = 'S';
                }
            }
        }

        $corrigido             = $mpergunta->verificarAnaliseCorrigida($oabid, $ordem, $obrid);
        $edicaoDesabilitada    = $mObra->habilitarEdicaoPorPerfilEstadoDocumento($obrid);
        $disabled = '';
        if ($edicaoDesabilitada == false) {
            $disabled = 'disabled';
        }

        if($esd['esdid'] != Par3_Model_Obra::ESDID_EM_CADASTRAMENTO) {
            if($pendenciaDadosBasicos && $disabled == '') {
                return '';
            }
            if($corrigido && $disabled == '') {
                return '';
            }
        }

        if($esd['esdid'] == Par3_Model_Obra::ESDID_EM_CADASTRAMENTO) {
            if ($pendenciaPreenchimento && $disabled == '') {
                return '';
            }
        }

        if(
            in_array($esd['esdid'],$mAnalise->estadosCorrecaoAnalise()) &&
            array_intersect($pflcod,$mAnalise->perfisCorrecaoAnalise())
        ){
            return '';
        }
        return 'disabled';
    }

    public function listarPendenciasPorAba($oabid, $obrid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);
        //VALIDA SE O ESTADO DOCUMENTO ATUAL DEVE MOSTRAR A ANÁLISE
        if(
            (!array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao()))   ||
            (!array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadoValidacao()))
        ){
            return true;
        }
        return $mAnalise->listarPendenciasPorAba($oabid, $obrid);
    }

    public function verificarPerfilAnalista()
    {
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);
        $perfisPermitidos = [
            Par3_Model_UsuarioResponsabilidade::COORDENADOR_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
        ];
        if(array_intersect($pflid,$perfisPermitidos)){
            return true;
        }
        return false;
    }

    public function carregarPerguntaDocumentosAnexos($otdid = null,$obrid = null)
    {
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();

        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        $pflid = pegaPerfilGeral($_SESSION['usucpf']);

        $permiteEditar = false;
        if((array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao()))||
            (array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadoValidacao()))){
            $permiteEditar = true;
        }

        if(in_array($esd['esdid'],$mAnalise->estadosCorrecaoAnalise()) || in_array($esd['esdid'],$mAnalise->estadoValidacao())){
            $aeg = $mAnalise->pegarUltimaAnalise($obrid);
            $aegid = $aeg['aegid'];
        }
        if(array_intersect($pflid,$mAnalise->perfisEdicao()) && in_array($esd['esdid'],$mAnalise->estadosEdicao())) {
            $aegid = $mAnalise->iniciarAnalise($obrid);
        }
        if(!$aegid){
            return false;
        }

        $mOtd = new Par3_Model_ObraTipoDocumento($otdid);
        $mAed = new Par3_Model_AnaliseEngenhariaObraDocumentosRespostas($aegid,$otdid);

        //Recupera resposta da última análise
        if($mAed->aed_observacao == null) {
            $aegidUltimaAnalise = $mAnalise->pegarUltimaAnalise($obrid)['aegid'];
            $mAerUltimaAnalise = new Par3_Model_AnaliseEngenhariaObraDocumentosRespostas($aegidUltimaAnalise,$otdid);

            $mAed->aed_observacao = $mAerUltimaAnalise->aed_observacao;
        }

        require APPRAIZ . 'par3/modulos/principal/planoTrabalho/obras/analise_engenharia/template_pergunta_documento_anexo.inc';
    }
}
?>