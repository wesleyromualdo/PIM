<?php

class Par3_Controller_AnaliseEngenhariaObra extends Controle
{

    public $model;

    public function __construct($aegid = null)
    {
        $this->model = new Par3_Model_AnaliseEngenhariaObra();
        $this->model->carregarPorId($aegid);
    }

    public function listar($obrid)
    {
        $sql = "SELECT 
                   aegid, 
                   aegid as sequencia,
                   to_char(aegdtcricacao, 'DD/MM/YYYY') as aegdtcricacao,
                   aegdtalteracao, 
                   aeg_finalizado, 
                   CASE WHEN aeg_finalizado = 't'
                      THEN 'Finalizado'
                      ELSE 'Em Análise'
                   END AS estado, 
                   usucpf
                FROM par3.analise_engenharia_obra aeg
                WHERE obrid = {$obrid} order by 1 asc";

        $cabecalho = ["Seq.", "Data de criação", "Situação"];
        $esconderColunas = ["usucpf", "aegdtalteracao", "aeg_finalizado"];

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('listaanaliseengenharia');
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('view', 'abrirModalInfo');
        $listagem->addAcao('print2', 'imprimirAnaliseEngenhariaObra');
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

        $lista = new Simec_Listagem();

    }

    /* Recupera Perguntas e respostas de todas as pendências da análise
      - aba análise de engenharia
    */
    public function recuperarPendenciasAnalise($aegid)
    {
        $sql = "SELECT DISTINCT -- Respostas Estudo de Demanda
                    aeg.aegid AS id_analise,
                    aepdsc AS pergunta,
                    aedo.aed_observacao AS resposta, -- resposta estudo
                    oab.oabid,
                    oabdsc AS aba
                FROM par3.analise_engenharia_obra aeg
                INNER JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aegid = aeg.aegid AND aer_pendencia = 'S'
                INNER JOIN par3.analise_engenharia_obra_aba_perguntas aep ON aep.aepid = aer.aepid 
                INNER JOIN par3.analise_engenharia_obra_estudo_demanda_observacao aedo ON aedo.aegid = aeg.aegid
                INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                WHERE aeg.aegid = {$aegid} AND oab.oabid = 1 -- aba estudo de demandas
UNION ALL
		SELECT DISTINCT -- Respostas Dados do terreno, relatório, fotos e cronograma
                    aeg.aegid AS id_analise,
                    aepdsc AS pergunta,
                    aer_observacao AS resposta,
                    oab.oabid,
                    oabdsc AS aba
                FROM par3.analise_engenharia_obra aeg
                INNER JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aegid = aeg.aegid AND aer_pendencia = 'S'
                INNER JOIN par3.analise_engenharia_obra_aba_perguntas aep ON aep.aepid = aer.aepid 
                INNER JOIN par3.obra_aba oab ON oab.oabid = aep.oabid
                WHERE aeg.aegid = {$aegid} 
                AND oab.oabid <> 1
UNION ALL
                SELECT distinct -- Respotas para Documentos Anexos
                    aeg.aegid AS id_analise,
                    COALESCE(otdanalise_engenharia, 'O documento informado está de acordo com as especificações exigidas?') as pergunta,
                    aed.aed_observacao AS resposta, -- resposta documentos
                    oab.oabid,
                    oabdsc AS aba
                FROM par3.analise_engenharia_obra aeg
                LEFT JOIN par3.analise_engenharia_obra_documentos_respostas aed ON aed.aegid = aeg.aegid AND aed_pendencia = 'S' 
                INNER JOIN par3.obra_tipo_documento otd ON otd.otdid = aed.otdid
                INNER JOIN par3.obra_aba oab ON oab.oabid = 7 -- aba documentos anexos
                WHERE aeg.aegid = {$aegid}";

        return $this->model->carregar($sql);
    }

    public function montarHtmlPendenciasAnalise($dados)
    {


        $mOed = new Par3_Model_ObraEstudoDemanda($_GET['obrid']); //Recuperar motivo estudo demanda
        $arrOed = $mOed->recuperarPorObra($_GET['obrid']);
        $octid = $mOed->recuperarCategoriaObra($_GET['obrid']);
        $octidint = $octid['octid'];


        $dadosPendenciasEstudoDemanda = $this->recuperarPendenciasEstudoDemanda($dados['aegid'], $arrOed['omoid'], $octidint, $_GET['obrid']);
        $dadosPendenciasRelatorio = $this->recuperarPendenciasRelatorio($dados['aegid'], $_GET['obrid']);
        $dadosPendenciasDadosTerrono = $this->recuperarPendenciasDadosTerreno($dados['aegid'], $_GET['obrid']);
        $dadosPendenciasAnexos = $this->recuperarPendenciasAnexos($dados['aegid'], $_GET['obrid']);
        $dadosPendenciasPlanilha = $this->recuperarPendenciasPlanilha($dados['aegid'], $_GET['obrid']);
        $dadosPendenciasFotos = $this->recuperarPendenciasFotos($dados['aegid'], $_GET['obrid']);


        //$modelObra = new Par3_Model_Obra($dados['obrid']);
        //$pendenciasAbas = $modelObra->verificarPendencias($dados['obrid']);
        $observacaoGeralEstudoDemanda = $this->recuperarComentarioPendenciasEstudoDemanda($dados['aegid']);

        $html  = $this->montaCabecalhoAnalista($dados['aegid']);
        $html .= $this->montadiv('Estudo de Demanda', $dadosPendenciasEstudoDemanda,$observacaoGeralEstudoDemanda);
        $html .= $this->montadiv('Dados do Terreno', $dadosPendenciasDadosTerrono);
        $html .= $this->montadiv('Relatório de Vistoria', $dadosPendenciasRelatorio);
        $html .= $this->montadiv('Fotos do Terreno', $dadosPendenciasFotos);
        $html .= $this->montadiv('Planilha Orçamentária', $dadosPendenciasPlanilha);
        $html .= $this->montadiv('Documentos em Anexo', $dadosPendenciasAnexos);
        //  $html .= $this->montadiv('Cronograma', $dadosPendenciasCronograma);


        echo $html;


    }

    /**
     * @param $dadosUsuario
     * @return string
     */
    public function montaCabecalhoAnalista($aegid)
    {
        $aeg = new Par3_Model_AnaliseEngenhariaObra($aegid);
        $usuario = new Seguranca_Model_Usuario($aeg->usucpf);
        $dataCriacao = formata_data_hora($aeg->aegdtcricacao);
        if($usuario->usunome) {
            $html = <<<HTML
            <div class="col-lg-6" align="center"><b>Analista:</b> {$usuario->usunome}</div>
            <div class="col-lg-6" align="center"><b>Data de Criação:</b> {$dataCriacao}</div>
            <hr>
HTML;
        }
        return $html;
    }

    public function montadiv($nomeDiv, $parametroForeache,$observacoes = null)
    {
        if(!$parametroForeache){
            return '';
        }
        $html  = "<br>";
        $html .= "<div>";
        $html .= "<h3>" . $nomeDiv . "</h3><hr>";
        $html .= "<div class='row'>";
        $html .= "<ul class='list-group' >";
        $html .= '<br>';
        foreach ($parametroForeache as $item) {
            $pendencia = ($item['aer_pendencia'] == 'S' ? 'Não':'Sim');
            $observacao = ($item['aer_observacao']?$item['aer_observacao']:'N/A');
            $html .= "<li class='list-group-item'>";
            $html .= "<h4 class='list-group-item-heading' style='page-break-inside:avoid; page-break-after:auto;'>{$item['aepdsc']}</h4>";
            $html .= "<p class='list-group-item-text'>{$pendencia}</p>";
            $html .= "<div>";
            $html .= "<p class='list-group-item-text'><b>Observação:</b> {$observacao}</p>";
            $html .= "</div></li>";
        }
        if($observacoes) {
            $html .= "<div class='row' style='page-break-inside:avoid; page-break-after:auto;'>";
            $html .= "<h4 align='center'>Observações Gerais {$nomeDiv}</h4>";
            $html .= "<p align='left' style='margin:20px;border:solid gray .5px;padding:10px;'>";
            $html .= "<b>{$observacoes}</b>";
            $html .= "</p>";
            $html .= "</div>";
        }
        $html .= "</div>";
        $html .= "</ul><br></div>";
        $html .= "<div></div>";
        return $html;
    }


    public function recuperarNomeResponsavelAnalise($aegid)
    {
        $sql = "SELECT usunome 
                FROM seguranca.usuario usu
                INNER JOIN par3.analise_engenharia_obra aeg ON aeg.usucpf = usu.usucpf
                WHERE aeg.aegid = {$aegid}";

        return $this->model->pegaUm($sql);
    }


    public function recuperarPendenciasEstudoDemanda($aegid, $arrOed, $octid, $obrid)
    {
        if ($octid == 1) {
            switch ($arrOed) {
                case 1:
                    $sql = " SELECT DISTINCT  est.obrid , ('O motivo da Contrução está coerente com a proposta apresentada?') as aepdsc,  (case when aer.aer_observacao  is null then 'Sim' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid} 
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 1
                        
                         UNION ALL
                         
                         SELECT DISTINCT est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 2
                        
                         
                        UNION ALL
                        
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem ,aer_pendencia
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 3
                        
                         
                        UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 4
                        
                              UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 5
                        
      
                        UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 8
                   
                         
                         order by aepordem";
                    break;
                case 2:
                    $sql = " 
                      SELECT DISTINCT est.obrid , ('O motivo da Contrução está coerente com a proposta apresentada?') as aepdsc,  (case when aer.aer_observacao  is null then 'Sim' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 1
                        
                         UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 2
                        
                         
                        UNION ALL
                        
                         SELECT DISTINCT est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 3
                        
                         
                        UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 4
                        
                 
                        UNION ALL
                         
                         SELECT DISTINCT est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 8

                      
                         order by aepordem";


                    break;
                default;
                    break;
            }


            return $this->model->carregar($sql);

        } else if ($octid == 2) {

            switch ($arrOed) {
                case 1:

                    $sql = "  SELECT DISTINCT  est.obrid , ('O motivo da Ampliação está coerente com a proposta apresentada?') as aepdsc,  (case when aer.aer_observacao  is null then 'Sim' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 1
                        
                         UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 2
                      
                         
                        UNION ALL
                        
                         SELECT DISTINCT est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 3
    
                        UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 5
                        
                         
                        UNION ALL
                          
                         SELECT DISTINCT est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 7
                           UNION ALL
                          
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 8
                         
                         
                      ";

                    break;
                case 2:
                    $sql = " 
                         SELECT DISTINCT  est.obrid , ('O motivo da Ampliação está coerente com a proposta apresentada?') as aepdsc,  (case when aer.aer_observacao  is null then 'Sim' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 1
                        
                         UNION ALL
                         
                         SELECT DISTINCT est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 2
                        
                         
                        UNION ALL
                        
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 3
             
                         
                        UNION ALL
                          
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = 1224
                         inner join par3.obra_estudo_demanda est on est.obrid =  1224
                         where oab.oabid = 1 and aepordem = 7
                           UNION ALL
                          
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 8
                         
                         
                      ";


                    break;
                case 3:
                    $sql = " SELECT DISTINCT  est.obrid , ('O motivo da Ampliação está coerente com a proposta apresentada?') as aepdsc,  (case when aer.aer_observacao  is null then 'Sim' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 1
                        
                         UNION ALL
                         
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 9
                        
                         
                        UNION ALL
                        
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null then 'N/A' end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 5
             
                         
                        UNION ALL
                          
                         SELECT DISTINCT  est.obrid ,aepdsc,  (case when aer.aer_observacao  is null or length(aer_observacao) <= 1 then 'N/A' else aer.aer_observacao  end) as  aer_observacao, oab.oabid ,aepordem,aer_pendencia 
                         FROM par3.analise_engenharia_obra_aba_perguntas aep
                         left JOIN par3.analise_engenharia_obra_aba_respostas aer ON aer.aepid = aep.aepid and aer.aegid = {$aegid}  
                         INNER JOIN par3.obra_aba oab on oab.oabid = aep.oabid
                         inner join par3.obra obr on obr.obrid = {$obrid}
                         inner join par3.obra_estudo_demanda est on est.obrid =  {$obrid}
                         where oab.oabid = 1 and aepordem = 8";

                    break;
                default;
                    break;
            }


            return $this->model->carregar($sql);

        }

    }

    public function recuperarComentarioPendenciasEstudoDemanda($aegid)
    {
        $sql = "select aed_observacao from par3.analise_engenharia_obra_estudo_demanda_observacao where aegid = {$aegid}";
        return $this->model->pegaUm($sql);
    }

    public function recuperarPendenciasDadosTerreno($aegid, $obrid)
    {
        $sql = "
            SELECT  
                 obrid,
                 aepdsc, 
                 (case when aer_observacao  is null then 'N/A' else aer_observacao  end) as  aer_observacao,
                 aepordem,aer_pendencia 
            from par3.analise_engenharia_obra
            left join par3.analise_engenharia_obra_aba_respostas using(aegid)
            left join par3.analise_engenharia_obra_aba_perguntas using(aepid)
            where obrid = {$obrid}
            and oabid = 2
            and aegid = {$aegid}
        ";
        return $this->model->carregar($sql);
    }

    public
    function recuperarPendenciasRelatorio($aegid, $obrid)
    {
        $sql = "
            SELECT  
                 obrid,
                 aepdsc, 
                 (case when aer_observacao  is null then 'N/A' else aer_observacao  end) as  aer_observacao,
                 aepordem,aer_pendencia 
            from par3.analise_engenharia_obra
            left join par3.analise_engenharia_obra_aba_respostas using(aegid)
            left join par3.analise_engenharia_obra_aba_perguntas using(aepid)
            where obrid = {$obrid}
            and oabid = 3
            and aegid = {$aegid}";
        return $this->model->carregar($sql);
    }

    public
    function recuperarPendenciasFotos($aegid, $obrid)
    {
        $sql = "
            SELECT  
                 obrid,
                 aepdsc, 
                 (case when aer_observacao  is null then 'N/A' else aer_observacao  end) as  aer_observacao,
                 aepordem,aer_pendencia 
            from par3.analise_engenharia_obra
            left join par3.analise_engenharia_obra_aba_respostas using(aegid)
            left join par3.analise_engenharia_obra_aba_perguntas using(aepid)
            where obrid = {$obrid}
            and oabid = 4
            and aegid = {$aegid}";

        return $this->model->carregar($sql);


    }

    public
    function recuperarPendenciasCronograma($aegid, $obrid)
    {
        $sql = "
        SELECT  
                 obrid,
                 aepdsc, 
                 (case when aer_observacao  is null then 'N/A' else aer_observacao  end) as  aer_observacao,
                 aepordem,aer_pendencia 
            from par3.analise_engenharia_obra
            left join par3.analise_engenharia_obra_aba_respostas using(aegid)
            left join par3.analise_engenharia_obra_aba_perguntas using(aepid)
            where obrid = {$obrid}
            and oabid = 6
            and aegid = {$aegid}
        ";

        return $this->model->carregar($sql);

    }

    public
    function recuperarPendenciasAnexos($aegid, $obrid)
    {
        global $db;

        $sql = "  SELECT distinct -- Respotas para Documentos Anexos
                    aeg.aegid AS id_analise,
                    COALESCE(otdanalise_engenharia, 'O documento informado está de acordo com as especificações exigidas?') as aepdsc,
                    aed.aed_observacao as aer_observacao, -- resposta documentos
                    oab.oabid,
                    aed.aed_pendencia as aer_pendencia,
                    oabdsc AS aba
                FROM par3.analise_engenharia_obra aeg
                LEFT JOIN par3.analise_engenharia_obra_documentos_respostas aed ON aed.aegid = aeg.aegid
                INNER JOIN par3.obra_tipo_documento otd ON otd.otdid = aed.otdid
                INNER JOIN par3.obra_aba oab ON oab.oabid = 7 -- aba documentos anexos
                WHERE aeg.aegid = {$aegid}";

        if(pg_num_rows($db->executar($sql)) == 0) {

           return array(
                        array('id_analise'=>'',
                              'aepdsc' => 'O documento informado está de acordo com as especificações exigidas?',
                              'aer_observacao' => 'N/A',
                              'aer_pendencia' => 'Sim',
                              'oabid'=>'7',
                             'aba'=>'Documentos Anexos)'));

        }

        return $this->model->carregar($sql);
    }

    function recuperarPendenciasPlanilha($aegid, $obrid)
    {
        $sql = "
            SELECT  
                 obrid,
                 aepdsc, 
                 (case when aer_observacao  is null then 'N/A' else aer_observacao  end) as  aer_observacao,
                 aepordem,aer_pendencia 
            from par3.analise_engenharia_obra
            left join par3.analise_engenharia_obra_aba_respostas using(aegid)
            left join par3.analise_engenharia_obra_aba_perguntas using(aepid)
            where obrid = {$obrid}
            and oabid = 5
            and aegid = {$aegid}
        ";
        return $this->model->carregar($sql);
    }

    public function atualizarHistoricoEntidades($obrid = null)
    {
        $whereObrid = '';
        if($obrid) {
            $whereObrid = 'where obr.obrid = '.$obrid;
        }

        $db = new cls_banco();

        $sql = <<<SQLOBRA
  select obr.obrid
  from par3.obra obr
  inner join par3.analise_engenharia_obra aeg on aeg.obrid = obr.obrid
  $whereObrid
SQLOBRA;

        $obras = $db->carregar($sql);
        $sqls = '';
        /**
         * listar todas as obras com análise
         *
         */
        $aObrid    = [];
        foreach($obras as $obra) {
            $historico = <<<historico
    select
        hs.hstid  
    from par3.analise_engenharia_obra aeg
    inner join par3.obra obr on obr.obrid = aeg.obrid
    inner join workflow.historicodocumento hs on hs.docid = obr.docid 
    inner join workflow.acaoestadodoc aed on aed.aedid = hs.aedid
      and aed.esdidorigem = aeg.esdid
      and hs.aedid = aed.aedid
    where obr.obrid = {$obra['obrid']}
    group by hs.hstid,obr.obrid
    order by 1 asc
historico;

            $rsHistorico = $db->carregar($historico);
            if(!is_array($rsHistorico) || count($rsHistorico) == 0) {
                continue;
            }

            $rsHistorico = array_values($rsHistorico);

            $sqlAnalise = <<<ANALISE
    select
        aegid
    from
    par3.analise_engenharia_obra
    where obrid = {$obra['obrid']}
    and aeg_finalizado is not null
    order by aegid asc 
ANALISE;

            $rsAnalise = $db->carregar($sqlAnalise);
            if(!is_array($rsAnalise)) {
                continue;
            }
            $rsAnalise = array_values($rsAnalise);

            $sqlUpdate = [];
            $arAegid = [];
            for($i=0;$i < count($rsAnalise);$i++) {
                $obrid  = $obra['obrid'];
                $hstid = $rsHistorico[$i]['hstid'];
                $aegid = $rsAnalise[$i]['aegid'];
                if($hstid == '' || $aegid == '') {
                    continue;
                }
                $sqlUpdate[] = "UPDATE par3.analise_engenharia_obra set hstid = {$hstid} where aegid = {$aegid} and obrid = {$obrid}";
                $arAegid[] = $aegid;
                unset($hstid);
                unset($aegid);
            }

            $aObrid[] = [
                'obrid' => $obra['obrid'],
                'analises' => $arAegid
            ];

            if(count($sqlUpdate) == 0) {
                continue;
            }

            $sqls   = implode(';',$sqlUpdate);
            $sqls  .= ';';
            $db->executar($sqls);
            $db->commit();
            unset($sqlUpdate);
            unset($sqls);
        }

        //Dados padrão para envio de email para os nutricionistas
        $strAssunto = 'Atualização do histórico de análises';

        $stTableObras  = '<table style="border: 1px solid black;border-collapse: collapse;text-align: justify;" border="3">';
        $stTableObras .= '<tr><td align="center">QTD</td><td align="center">Obra</td><td align="center">Análises Atualizadas</td></tr>';
        $cont = 1;
        foreach($aObrid as $obra) {
            $analises = implode(',',$obra['analises']);
            $stTableObras .= "<tr>";
            $stTableObras .= "<td align='center'>{$cont}</td>";
            $stTableObras .= "<td align='center'>{$obra['obrid']}</td>";
            $stTableObras .= "<td align='center'>{$analises}</td>";
            $stTableObras .= "</tr>";
            $cont++;
        }
        $stTableObras .= '</table>';


        $strMensagem = <<<EMAIL
    <pre align="center" style="text-align: justify;">
        <p>Lista de Entidades Atualizadas:</p>
        {$stTableObras}
    </pre>
EMAIL;

        $strEmailTo  = array('daniel.fiuza@mec.gov.br');
        if ($_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
            $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }

        if($_SERVER['HTTP_HOST'] == "simec.mec.gov.br") {
            $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }

        return $stTableObras;
    }
}