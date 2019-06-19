<?php
/**
 * Classe de mapeamento da entidade par3.analise_engenharia_obra.
 *
 * @version $Id$
 * @since 2017.11.03
 */

/**
 * Par3_Model_Analise_engenharia_obra: Tabela utilizada para cadastro de perguntas para o questionário de análise de engenharia
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Analise_engenharia_obra($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Analise_engenharia_obra($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $aegid  - default: nextval('par3.analise_engenharia_obra_aegid_seq'::regclass)
 * @property int $obrid FK da Análise (par3.obra)
 * @property \Datetime(Y-m-d H:i:s) $aegdtcricacao campo usado para armazenar a data de criação da análise de engenharia
 * @property \Datetime(Y-m-d H:i:s) $aegdtalteracao campo usado para armazenar a alteração da análise de engenharia - default: now()
 * @property int usucpf
 */
class Par3_Model_AnaliseEngenhariaObra extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aegid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aegid' => null,
        'obrid' => null,
        'aegdtcricacao'  => null,
        'aegdtalteracao' => null,
        'aeg_finalizado' => null,
        'esdid'          => null,
        'usucpf'         => null,
    );


    public function verificaAnalise($obrid)
    {
//        require_once APPRAIZ . '/includes/workflow.php';
//        $mObr = new Par3_Model_Obra($obrid);
//        return $mObr->docid == 2040 && wf_verificarPerfil(2040)? true : false;
//        return true;
        $pflid = pegaPerfil($_SESSION['usucpf']);
        $perfisPermitidos = [
            Par3_Model_UsuarioResponsabilidade::COORDENADOR_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
        ];
        if(in_array($pflid,$perfisPermitidos)){
            return true;
        }
        return false;
    }

    public function iniciarAnalise($obrid)
    {
        $permiteEscrever = $this->permiteEscreverAnalise($obrid);
        if(!$permiteEscrever){
            return false;
        }
        require_once APPRAIZ . '/includes/workflow.php';
        $mObra = new Par3_Model_Obra($obrid);
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE obrid = {$obrid}  and  aeg_finalizado IS NULL";
        $analise = $this->recuperar($sql);
        $aegid   = $analise['aegid'];
        if(!$analise){
            $dados['aegdtcricacao'] = date('Y-m-d H:m:s');
            $dados['obrid']         = $obrid;
            $dados['esdid']         = $esd['esdid'];
            $dados['usucpf']        = $_SESSION['usucpf'];
            try{
                $this->popularDadosObjeto($dados);
                $aegid = $this->salvar();
                $this->commit();
            } catch (Simec_Db_Exception $e) {
                return $aegid;
            }
            return $aegid;
        }
        $dados['aegid']          = $aegid;
        $dados['aegdtalteracao'] = date('Y-m-d H:m:s');
        $this->popularDadosObjeto($dados);
        $aegid = $this->salvar();
        return $aegid;
    }

    public function pegarUltimaAnalise($obrid)
    {
//        $sql = "select  aegid from par3.analise_engenharia_obra where obrid = {$obrid} order by aegdtcricacao desc limit 1;";
        $sql = <<<DML
        with analise as (
                select
                aeg.obrid,
                oab.oabdsc,
                esd.esddsc,
                aeg.aegid,
                aep.aepdsc as descricao,
                aegdtcricacao
                from par3.analise_engenharia_obra aeg 
                left join par3.analise_engenharia_obra_aba_respostas aer on aeg.aegid = aer.aegid
                left join par3.analise_engenharia_obra_aba_perguntas aep on aep.aepid = aer.aepid
                left join par3.obra_aba oab on oab.oabid = aep.oabid
                left join workflow.estadodocumento esd on esd.esdid = aeg.esdid
                --where aer.aer_pendencia = 'S'
            
            union all 
                
                select
                aeg.obrid,
                'Documentos Anexos' as descricao,
                esd.esddsc,
                aeg.aegid,
                otd.otdanalise_engenharia as descricao,
                aegdtcricacao
                from par3.analise_engenharia_obra aeg 
                left join par3.analise_engenharia_obra_documentos_respostas aed on aed.aegid = aeg.aegid
                left join par3.obra_tipo_documento otd on otd.otdid = aed.otdid
                left join workflow.estadodocumento esd on esd.esdid = aeg.esdid
                --where aed.aed_pendencia = 'S'

            union all
                
                select
                aeg.obrid,
                oab.oabdsc,
                esd.esddsc,
                aeg.aegid,
                aep.aepdsc as descricao,
                aegdtcricacao
                from par3.analise_engenharia_obra aeg 
                left join par3.analise_engenharia_obra_aba_respostas aer on aeg.aegid = aer.aegid
                left join par3.analise_engenharia_obra_estudo_demanda_observacao obs on obs.aegid = aeg.aegid 
                left join par3.analise_engenharia_obra_aba_perguntas aep on aep.aepid = aer.aepid
                left join par3.obra_aba oab on oab.oabid = aep.oabid
                left join workflow.estadodocumento esd on esd.esdid = aeg.esdid
                where oab.oabid = 1
                --and aer.aer_pendencia = 'S'
        )
            
        select aegid from analise where obrid = {$obrid} and descricao is not null
        order by aegdtcricacao desc
        limit 1
DML;
        return $this->recuperar($sql);
    }

    public function recuperarAnalisePorObra($obrid)
    {
        //        descomentar para testar com a tramitação
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE obrid = {$obrid} and  aeg_finalizado IS NULL";

        if(in_array($esd['esdid'],$this->estadosCorrecaoAnalise()) || in_array($esd['esdid'],$this->estadoValidacao())){
            return $this->pegarUltimaAnalise($obrid);
        }
        $analise = $this->recuperar($sql);
        return $analise;
    }

    public function validarAnaliseDadosTerreno($obrid,$arrPost)
    {

        $mAep1     = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas(Par3_Model_ObraAba::DADOS_TERRENO, 1);
        $mAep2     = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas(Par3_Model_ObraAba::DADOS_TERRENO, 2);
        $mAep3     = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas(Par3_Model_ObraAba::DADOS_TERRENO, 3);
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        $aeg      = $this->recuperarAnalisePorObra($obrid);
        $mAer1 = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aeg['aegid'],$mAep1->aepid);
        $mAer2 = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aeg['aegid'],$mAep2->aepid);
        $mAer3 = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aeg['aegid'],$mAep3->aepid);

        $mObr = new Par3_Model_Obra($obrid);

        if($mAer1->aer_pendencia == 'S'){
            if(
                $mObr->ovpid       == $arrPost['ovpid']      &&
                $mObr->tplid       == $arrPost['tplid']      &&
                $mObr->obrdsc      == $arrPost['obrdsc']     &&
                $mObr->obrmetragem == $arrPost['obrmetragem']
            ){
                return false;
            }
            $mAer1->popularDadosObjeto(['aer_corrigido' => 'S','aer_pendencia' => 'N']);
            $mAer1->salvar();
            $mAer1->commit();
        }

        if($mAer2->aer_pendencia == 'S'){
            if(
                $mObr->obrcep              == $arrPost['obrcep']              &&
                $mObr->obrlogradouro       == $arrPost['obrlogradouro']       &&
                $mObr->obrnumero           == $arrPost['obrnumero']           &&
                $mObr->obrcomplemento      == $arrPost['obrcomplemento']      &&
                $mObr->obrponto_referencia == $arrPost['obrponto_referencia'] &&
                $mObr->obrbairro           == $arrPost['obrbairro']
            ){
                return false;
            }
            $mAer2->popularDadosObjeto(['aer_corrigido' => 'S','aer_pendencia' => 'N']);
            $mAer2->salvar();
            $mAer2->commit();
        }

        if($mAer3->aer_pendencia == 'S'){
            $mAer3->popularDadosObjeto(['aer_corrigido' => 'S','aer_pendencia' => 'N']);
            $mAer3->salvar();
            $mAer3->commit();
        }
        return true;
    }

    public function verificarSituacaoResponderAnalise($obrid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);
        if(!array_intersect($pflid,$this->perfisCorrecaoAnalise()) || !in_array($esd['esdid'],$this->estadosCorrecaoAnalise())){
            return false;
        }

        return true;
    }

    /**Retorna array dos estados habilitados para a edição da Análise de engenharia
     * @return array
     */
    public function estadosEdicao()
    {
        $estadosDocumentoEdicao = [
            Par3_Model_Obra::ESDID_EM_ANALISE,
            Par3_Model_Obra::ESDID_AGUARDANDO_ANALISE_FNDE_RETORNO_DILIGENCIA,
            Par3_Model_Obra::ESDID_EM_REVISAO_ANALISE_RETORNO_PROFE,
            Par3_Model_Obra::ESDID_REVISAO_ANALISE,
            Par3_Model_Obra::ESDID_ANALISE_DE_REFORMULACAO,
        ];
        return $estadosDocumentoEdicao;
    }

    /**Retorna a lista de perfis habilitados para a edição da Análise de Engenharia
     * @return array
     */
    public function perfisEdicao()
    {
        $perfisPermitidos = [
            Par3_Model_UsuarioResponsabilidade::COORDENADOR_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
        ];
        return $perfisPermitidos;
    }

    public function perfisSuperUsuario()
    {
        $perfisPermitidos = [
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
        ];
        return $perfisPermitidos;
    }

    public function estadoValidacao()
    {
        $estados = [
            Par3_Model_Obra::ESDID_REVISAO_ANALISE,
            Par3_Model_Obra::ESDID_AGUARDANDO_ANALISE_FNDE_RETORNO_DILIGENCIA,
            Par3_Model_Obra::ESDID_VALIDACAO_DILIGENCIA,
        ];
        return $estados;
    }

    public function perfisCorrecaoAnalise()
    {
        $perfisHabilitados = [
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
            Par3_Model_UsuarioResponsabilidade::PREFEITO ,
            Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
            Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL,
            Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::COORDENADOR_TECNICO_CGEST,
        ];
        return $perfisHabilitados;
    }

    public function estadosCorrecaoAnalise()
    {
        $estadosHabilitados = [
            Par3_Model_Obra::ESDID_EM_DILIGENCIA,
            Par3_Model_Obra::ESDID_EM_DILIGENCIA_DE_REFORMULACAO
        ];
        return $estadosHabilitados;
    }

    public function permiteEscreverAnalise($obrid)
    {
        //        descomentar para testar com a tramitação
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
//        $arrSituacoesHabilitadas = [
//            Par3_Model_Obra::ESDID_EM_ANALISE,
//            Par3_Model_Obra::ESD,
//        ];
//
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);
//        $perfisPermitidos = [
//            Par3_Model_UsuarioResponsabilidade::COORDENADOR_TECNICO_CGEST,
//            Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
//            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
//            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
//        ];
        //VALIDA SE O ESTADO DOCUMENTO ATUAL DEVE MOSTRAR A ANÁLISE
        if(array_intersect($pflid,$this->perfisEdicao()) && (in_array($esd['esdid'],$this->estadosEdicao()) || in_array($esd['esdid'],$this->estadoValidacao()))){
            return true;
        }
        return false;
    }

    public function mostrarPendenciasEmAnalise($obrid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);

        $perfisSuperUsuario = [
            Par3_Model_UsuarioResponsabilidade::COORDENADOR_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
        ];

        if(!array_intersect($pflid,$perfisSuperUsuario) && !in_array($esd['esdid'],$this->estadosCorrecaoAnalise())){
            return false;
        }
        return true;
    }

    public function validarAnaliseEstudoDemanda($obrid,$ordem,$oedid = null)
    {
        $mObra = new Par3_Model_Obra($obrid);
        if(!$mObra->obrid) {
            return false;
        }

        if(!$this->verificarSituacaoResponderAnalise($obrid)){
            return false;
        }

        $mAep2 = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas(Par3_Model_ObraAba::ESTUDO_DEMANDA, $ordem);
        $aeg   = $this->recuperarAnalisePorObra($obrid);
        $mAer2 = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aeg['aegid'],$mAep2->aepid);
        if(!$mAer2->aerid){
            return false;
        }
        $mAer2->executar("UPDATE par3.analise_engenharia_obra_aba_respostas SET aer_corrigido = 'S',aer_pendencia = 'N' WHERE aerid = {$mAer2->aerid}");
        $mAer2->commit();
    }

    public function validarAnalisePlanilha($obrid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        if(!$mObra->obrid) {
            return false;
        }

        if(!$this->verificarSituacaoResponderAnalise($obrid)){
            return false;
        }

        $mAep2 = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas(Par3_Model_ObraAba::PLANILHA_ORCAMENTARIA, 1);
        $aeg   = $this->recuperarAnalisePorObra($obrid);
        $mAer2 = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($aeg['aegid'],$mAep2->aepid);
        if(!$mAer2->aerid){
            return false;
        }
        $mAer2->executar("UPDATE par3.analise_engenharia_obra_aba_respostas SET aer_corrigido = 'S',aer_pendencia = 'N' WHERE aerid = {$mAer2->aerid}");
        $mAer2->commit();
    }

    public function verificarExistePendencia($obrid,$oabid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        $perfis = [Par3_Model_UsuarioResponsabilidade::PREFEITO ,Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL];
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);

        if(in_array($esd['esdid'],$this->estadosEdicao()) && array_intersect($pflid,$perfis)||
            in_array($esd['esdid'],$this->estadoValidacao()) && array_intersect($pflid,$perfis)
        ){
            return true;
        }

        $analise =  $this->recuperarAnalisePorObra($obrid);
        if(!$analise){
            return true;
        }

        $sql = "
                SELECT count(*) FROM par3.analise_engenharia_obra_aba_respostas aer
                INNER JOIN par3.analise_engenharia_obra_aba_perguntas aep on aep.aepid = aer.aepid
                AND   aegid = {$analise['aegid']}
                AND   aep.oabid = {$oabid}
                AND   aer.aer_pendencia = 'S' and (aer.aer_corrigido is null or aer.aer_corrigido = 'N')

        ";

        $pendencias = $this->carregar($sql);

        if($pendencias[0]['count'] == 0){
            return true;
        }
        return false;
    }

    public function verificarExistePendenciaDocumentos($obrid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        $perfis = [Par3_Model_UsuarioResponsabilidade::PREFEITO ,Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL];
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);

        if(in_array($esd['esdid'],$this->estadosEdicao()) && array_intersect($pflid,$perfis)||
            in_array($esd['esdid'],$this->estadoValidacao()) && array_intersect($pflid,$perfis)
        ){
            return true;
        }

        $analise =  $this->recuperarAnalisePorObra($obrid);
        if(!$analise){
            return true;
        }
        $sql = "
                SELECT count(*) FROM par3.analise_engenharia_obra_documentos_respostas aed
                WHERE   aed.aegid = {$analise['aegid']}
                AND   aed.aed_pendencia = 'S'
        ";

        $pendencias = $this->carregar($sql);
        if($pendencias[0]['count'] == 0){
            return true;
        }
        return false;
    }

    public function listarPendenciasPorAba($oabid, $obrid)
    {
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        $analise =  $mAnalise->recuperarAnalisePorObra($obrid);
        if(!$analise){
            return true;
        }
        $sql = "
                SELECT * FROM par3.analise_engenharia_obra_aba_respostas aer
                INNER JOIN par3.analise_engenharia_obra_aba_perguntas aep on aep.aepid = aer.aepid
                AND   aegid = {$analise['aegid']}
                AND   aep.oabid = {$oabid}
                ORDER BY aep.aepordem
        ";

        if($oabid ==  Par3_Model_ObraAba::DOCUMENTOS_ANEXOS){
            $sql = "
                SELECT aer.*,otd.otdanalise_engenharia,otd.otdanalise_textorico FROM par3.analise_engenharia_obra_documentos_respostas aer
                INNER JOIN  par3.obra_tipo_documento otd on otd.otdid = aer.otdid
                WHERE   aegid = {$analise['aegid']}
        ";
        }
        return $mAnalise->carregar($sql);
    }

    public function retirarPendencia($obrid, $aba, $ordem){

        if(!$this->verificarSituacaoResponderAnalise($obrid)){
            return false;
        }

        $analise =  $this->recuperarAnalisePorObra($obrid);
        $mAep1 = new Par3_Model_AnaliseEngenhariaObraAbaPerguntas($aba, $ordem);
        $mAer1 = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($analise['aegid'],$mAep1->aepid);
        $mAer1->popularDadosObjeto(['aer_corrigido' => 'S','aer_pendencia' => 'N']);
        $mAer1->salvar();
        $mAer1->commit();
    }
}
