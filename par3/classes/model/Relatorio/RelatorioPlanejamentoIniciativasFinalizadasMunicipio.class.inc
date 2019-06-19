<?php
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 10/20/2017
 * Time: 11:10 AM
 */


/**
 * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
 * Class Par3_Model_Relatorio_RelatorioPlanejamentoIniciativasFinalizadasMunicipio
 * @decription Esta classe é utilizada para criar o Relatório de Planejamemto das Iniciativas Finalizdas do Município
 * @proprety title
 */
class Par3_Model_Relatorio_RelatorioPlanejamentoIniciativasFinalizadasMunicipio extends Modelo{

    //atributos dq classe
    private $title;
    private $situacao;
    private $instrumentoUnidade;
    private $arquivo;
    private $estadoFinalizado = 2077;


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     */
    public function render(){
        $qt = $this->existeIniciativasFinalizadas();
        if($qt['qt'] > 0) {
            //Título
            $this->renderTitulo();
            // Valor total enviado para analise
            $this->renderSituacaoValorPlanejadoDaunidade();
            //Cabecalho Dimensão, Quantidade de Iniciativas por dimensão e valor total das iniciativas da dimensão
            echo $this->renderDadosQuantidadeIniciativasValorTotalByDimensao();
            //Descrição das iniciativas do planejamento de cada dimensão e seus valores por ano, com total no final
            echo $this->renderTabelaDescricaoIniciativaPorDimensao();
            //Detalhes das iniciativas
            $this->renderDetalhaIniciativasPorDimensao();
        }else{
            echo "<h4> A unidade não possui nenhuma iniciativa finalizada </h4>";
        }
    }

    public function setConfigCSS(){
        $this->insertArquivoRelatorio("css");
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return string
     * @decription Retona uma string para ser utilizada no not in, quando necessário a impressão, as iniciativas que possuem
     * os estados a seguir não entraram na impressão
     */
    private function getNotInEstadoDocumentoIniciativaPlanejamento(){
        $arr = implode(',',$this->estadoDocumentoNaoEntraRelatorio());
        return $arr;
    }

    /*****
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description esta função retorna um array com estados que não irão entrar na impressão da finalização das iniciativas planejadas
     *  1 = Em cadastramento
     *  2 = Cancelada
     *
     */
    private function estadoDocumentoNaoEntraRelatorio(){
        return array(
                    PAR3_ESDID_CADASTRAMENTO,
                    PAR3_AEDID_CANCELADA
        );
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return string
     */
    public function getTitle()
    {
        return $this->title;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param mixed $title
     * @return $this
     */
    public function setTitle($title)
    {
        $this->title = ($title != '')?$title:'Resumo Geral';
        return $this;
    }

    /**
     * @return mixed
     */
    public function getSituacao()
    {
        return $this->situacao;
    }

    /**
     * @param mixed $situacao
     * @return Par3_Model_Relatorio_RelatorioPlanejamentoIniciativasFinalizadasMunicipio
     */
    public function setSituacao($situacao)
    {
        $this->situacao = $situacao;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getInstrumentoUnidade()
    {
        return $this->instrumentoUnidade;
    }

    /**
     * @param mixed $instrumentoUnidade
     * @return Par3_Model_Relatorio_RelatorioPlanejamentoIniciativasFinalizadasMunicipio
     */
    public function setInstrumentoUnidade($instrumentoUnidade)
    {
        $this->instrumentoUnidade = $instrumentoUnidade;
        return $this;
    } // Título do Relatório string


    public function getDadosQuantidadeIniciativasValorTotalByDimensao(){
        if(isset($this->instrumentoUnidade)){
            $sql = "	
                    select distinct 
                        dimensao.dimid,
                        dimensao.dimdsc
                        from par3.dimensao as dimensao
                            inner join par3.instrumentounidade as inu on dimensao.itrid = inu.itrid
                         where inu.inuid = {$this->instrumentoUnidade}
                         and dimensao.dimstatus = 'A'
                         order by dimensao.dimid asc;";
            try{
                $result = $this->carregar($sql);
                return $result;
            }catch (Exeption $e){
                $e->getMessage();
            }
        }
    }

    public function renderTitulo(){
        if($this->getTitle() != ''){
            $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tableSituacaoValorTotalPlanejado.inc';
            if(file_exists($arquivo)){
                $titulo = $this->getTitle();
                include($arquivo);
            }else{
                throw new Exception("Arquivo: ".$arquivo." Não encontrado");
            }
        }
    }

    public function renderSituacaoValorPlanejadoDaunidade(){
            $situacaoValorTotalPlanejado = $this->getValorTotalPlanejado();
            if(isset($situacaoValorTotalPlanejado) && $situacaoValorTotalPlanejado != null && $situacaoValorTotalPlanejado != ''){
                $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tabelaSituacaoValorTotalPlanejado.inc';
                if(file_exists($arquivo)){
                    include($arquivo);
                }else{
                    throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                }
            }


    }



    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return string
     * @description Este método monta a primeira tabela do relátório, #Dimensão #Quantidade de Iniciativas Planejadas #Valor Total da Dimensão Finalizada
     *
     */
    public function renderDadosQuantidadeIniciativasValorTotalByDimensao(){
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $instrumentoUnidade = $modelInstrumentoUnidade->create($this->instrumentoUnidade);
        $tipo = ($instrumentoUnidade['itrid'] == 2)? ' - Municipal': ' - Estadual';
        $html = '';
        $countRow = 1;
        $result = $this->getDadosQuantidadeIniciativasValorTotalByDimensao();
        if(isset($result[0])){
            $html .= "<table style='border: 1px solid #000000; border-collapse: collapse;'>";
            $html .= "<thead>";
            $html .= "<tr>";
                $html .= "<th  style='border: solid 1px #000000;'>Dimenção</th>";
                $html .= "<th  style='border: solid 1px #000000;'>Quandidade de Iniciativas Planejadas</th>";
                $html .= "<th  style='border: solid 1px #000000;'>Valor Total da Dimensão Finalizada</th>";
            $html .= "</tr>";
            $html .= "</thead>";
            //$html .= "</tbody>";
            foreach($result as $dados){
                $dadosDimensao = $this->getQuantidadeValorPorDimensao($dados['dimid']);
                $qt = ($dadosDimensao['quantidade_iniciativas'] == 0 ||$dadosDimensao['quantidade_iniciativas'] == null)?0:$dadosDimensao['quantidade_iniciativas'];
                $html .= "<tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>";
                    $html .= "<td>".$countRow." - ".$dados['dimdsc'].$tipo."</td>";
                    $html .= "<td>{$qt}</td>";
                    $html .= "<td> R$".number_format($dadosDimensao['valor_total'],2,',','.')."</td>";
                $countRow++;
                $html .= "</tr>";
            }
           // $html .= "</tbody>";
            $html .= "<table>";
            $html .= "<br/>";
        }
        return $html;
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return string
     * @description Este método monta o valor total do planejamento
     *
     */
    public function getValorTotalPlanejado(){
        if($this->instrumentoUnidade){
            $valorTotalDoPlanejamento = $this->valorTotalPlanejado();
            if($valorTotalDoPlanejamento['vlt_iniciativas'] != null && $valorTotalDoPlanejamento['vlt_iniciativas'] != '' && $valorTotalDoPlanejamento['vlt_iniciativas'] != 0) {
                return $valorTotalDoPlanejamento['vlt_iniciativas'];
            }else{
                return 0;
            }
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return array|bool|mixed|NULL
     * @throws Exception
     * @description Esta função calcula o valor total enviado para análise
     */
    private function valorTotalPlanejado(){

            $sql = "
                    select sum(inp.inpvalor_planejado_total) as vlt_iniciativas from par3.iniciativa_planejamento as inp
                    inner join workflow.documento as doc on doc.docid = inp.docid
            		inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid 
                    where 
                    inp.inuid = {$this->instrumentoUnidade} and 
                    esd.esdid not in ({$this->getNotInEstadoDocumentoIniciativaPlanejamento()});";



//select 
//                    SUM(inpplaitecom.ipivalorreferencia * inpplaitecom.ipiquantidade) as vlt_iniciativas
//                    from par3.iniciativa_planejamento as inp
//                                    inner join par3.iniciativa_planejamento_item_composicao as inpplaitecom on inpplaitecom.inpid = inp.inpid
//                                    inner join par3.iniciativa_itenscomposicao_grupo as iniitegru on iniitegru.iigid = inpplaitecom.iigid
//                                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitegru.itcid
//                                    inner join par3.unidade_medida as unid on unid.uniid = intcom.uniid
//                                    inner join workflow.documento as doc on doc.docid = inp.docid
//            						inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid 
//                                    where inp.inuid = {$this->instrumentoUnidade} and esd.esdid = {$this->estadoFinalizado};";
            try{
               // ver($sql);
                $result = $this->pegaLinha($sql);
                return $result;
            }catch(Exception $e){
                throw new Exception($e->getMessage());
            }

    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     */
    public function getAnos(){
        $html = '';
        $result = $this->getAnosCadastradosNasDimensoes();
        if(count($result) > 0){
            for($i = 0; $i <= count($result); $i++){
                if($result[$i]['ano'] != ''){
                    $html .= "<th class=''>".$result[$i]['ano']."</th>";
                }
            }
        }
        return $html;
    }

    public function getAnosCadastradosNasDimensoes(){
        $sql = "select distinct 
                anos.iniano as ano
                from par3.dimensao as dimensao
                        inner join par3.instrumentounidade as inu on dimensao.itrid = inu.itrid
                        inner join par3.iniciativa_planejamento as inp on inp.inuid = inu.inuid
                        inner join par3.iniciativa_iniciativas_anos as anos on anos.iniid = inp.iniid
                        inner join workflow.documento as doc on doc.docid = inp.docid
						inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    where inu.inuid = {$this->instrumentoUnidade} and  esd.esdid not in ({$this->getNotInEstadoDocumentoIniciativaPlanejamento()}) 
                    order by anos.iniano asc;";
        try{
        $result =  $this->carregar($sql);
        return $result;
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return array|mixed|NULL
     * @throws Exception
     * @description Carrega todas as dimensões que possuem alguma iniciativa planejada
     */
    public function getDimensaoCadastradaUnidade(){
        $sql = "select distinct
                dimensao.dimid,
                dimensao.dimdsc
                  from par3.iniciativa_planejamento as iniciativaplanejamento
                    inner join par3.dimensao as dimensao on dimensao.dimid = iniciativaplanejamento.dimid
                    inner join par3.instrumentounidade as instrumentounidade on instrumentounidade.inuid = iniciativaplanejamento.inuid
                    inner join par3.iniciativa as iniciativa on iniciativa.iniid = iniciativaplanejamento.iniid
                    inner join par3.iniciativa_descricao iniciativadescricao on iniciativa.indid = iniciativadescricao.indid
                where instrumentounidade.inuid = {$this->instrumentoUnidade}
                group by dimensao.dimid, iniciativaplanejamento.inpid,iniciativadescricao.inddsc
                order by dimensao.dimid asc";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }

    /**
     * @param $dimensao
     * @return array|mixed|NULL
     * @throws Exception
     */
    public function getLinhaDetalhaIniciativasPlanejadasByDimensao($dimensao){
        $sql = "select distinct 
                    iniciativa.iniid,
                    iniciativa.iniid || ' - ' ||iniciativadescricao.inddsc as descricaoiniciativa,
                    dimensao.dimid||' - '||dimensao.dimdsc as dimensaodesc,
                    dimensao.dimid,
                    (select count(*) 
                            from par3.iniciativa_planejamento as inp 
                                where inp.iniid = iniciativa.iniid and inp.dimid = dimensao.dimid and inp.inuid = instrumentounidade.inuid) as qt_iniciativas,
                                
                    (select SUM(inp.inpvalor_planejado_total) 
                            from par3.iniciativa_planejamento as inp 
                                where inp.iniid = iniciativa.iniid and inp.dimid = dimensao.dimid and inp.inuid = instrumentounidade.inuid) as qt_iniciativas
                    from par3.iniciativa_planejamento as iniciativaplanejamento 
                    inner join par3.instrumentounidade as instrumentounidade on instrumentounidade.inuid = iniciativaplanejamento.inuid
                    inner join par3.iniciativa as iniciativa on iniciativa.iniid = iniciativaplanejamento.iniid
                    inner join par3.iniciativa_descricao as iniciativadescricao on iniciativa.indid = iniciativadescricao.indid
                    inner join par3.dimensao as dimensao on dimensao.dimid = iniciativaplanejamento.dimid
                    
                    where 
                    iniciativaplanejamento.dimid = {$dimensao}
                    and 
                    iniciativaplanejamento.inpstatus = 'A'
                    and 
                    instrumentounidade.inuid  = {$this->instrumentoUnidade}";
                try{
                    $resultQUERY = $this->carregar($sql);
                    return $resultQUERY;
                }catch (Exception $e){
                    throw new Exception($e->getCode()." - ".$e->getMessage());
                }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $resultQUERY
     * @return string
     * @description Esta função monta uma linha da segunda tabela do relatório de finalização do planejamento
     */
    public function htmlLinhaDetalhaIniciativaIniciativas($resultQUERY){
        $arrDimensoes = $this->getDimensaoCadastradaUnidade();
        $html ='';
        for($i = 0; $i < count($resultQUERY); $i++){
            $html .= '<tr>';
            $html .= ($i > 0)?" ":"<td class='text-center' rowspan='{$resultQUERY[$i]['qt_iniciativa']}'>".$resultQUERY[$i]['dimdsc']."</td>";
            $html .= '<td>'.$resultQUERY[$i]['inddsc']."</td>";
            $html .= ($i > 0)?" ":"<td class='text-center' rowspan='{$resultQUERY[$i]['qt_iniciativa']}'>".$resultQUERY[$i]['qt_iniciativa']."</td>";
            $html .= $this->htmlValorIniciativaPorAno($resultQUERY[$i]['inpid']);
            $html .= ($i > 0)?" ":"<td class='text-center' rowspan='{$resultQUERY[$i]['qt_iniciativa']}'> R$".number_format($resultQUERY[$i]['vl_total_iniciativa_planejada'],'2',',','.')."</td>";
            $html .= '</tr>';
        }
        return $html;
    }


    public function htmlValorIniciativaPorAno($inpid){
        $arrAnos = $this->getAnosCadastradosNasDimensoes();
        $html = '';
        if(is_array($arrAnos)){
            foreach($arrAnos as $key => $values){
                $valorIniciativa= $this->getValorIniciativaIniciativaPorAno($inpid,$values['ano']);
                $novoValor = ($valorIniciativa == '' || is_null($valorIniciativa))?"R$00,00": "R$".$valorIniciativa;
                $html .= "<td>".$novoValor."</td>";
            }
        }
        return $html;
    }

    /**
     * @param $inpid
     * @param $iniano
     * @return mixed
     * @throws Exception
     */
    public function getValorIniciativaIniciativaPorAno($inpid,$iniano){
        $sql = "select 
                    SUM((itenscomposicao.ipiquantidade * itenscomposicao.ipivalorreferencia)) as vl
                        from par3.iniciativa_planejamento_item_composicao as itenscomposicao
                        inner join par3.iniciativa_planejamento as iniciativaplanejamento on iniciativaplanejamento.inpid = itenscomposicao.inpid
                        where itenscomposicao.inpid = {$inpid} and itenscomposicao.ipiano = {$iniano}";
        try{
            $result = $this->pegaLinha($sql);
            return $result['vl'];
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }
    }


    public function valorIniciativaPorAno($inip,$ano){
        if(isset($inip) && isset($ano)){
            $sql = "select inp.inpvalor_planejado_total from par3.iniciativa_planejamento as inp
                      inner join par3.iniciativa_iniciativas_anos as anos on anos.iniid = inp.inpid
                    where inp.inpid = {$inip} and anos.iniano = {$ano}";
            try{
                $result = simec_preparar_array($this->carregar($sql));
                return $result['inpvalor_planejado_total'];
            }catch (Exception $e){
                throw new Exception($e->getMessage());
            }
        }
    }



    /**
     * segunda tabela
     * 2.0
     */
    public function htmlTabelaDescricaoIniciativaPorDimensao(){
        $arrDimensoes = $this->getDadosQuantidadeIniciativasValorTotalByDimensao(); //pega todas as dimensões
        $html = '';
        $countRow = 1;
        //monta linhas
        foreach($arrDimensoes as $key => $values){
            $dadosIniciativasDaDimensao = $this->getQuantidadeValorPorDimensao($values['dimid']);
            if(count($dadosIniciativasDaDimensao['quantidade_iniciativas'])>0){
                $html .= $this->montaLinhaIniciativasByDimensao($values['dimid'],$countRow);
            }else{
                $html .= $this->htmlMontaLinhaVazia($values['dimid'],$countRow);
            }
            $countRow++;
        }
        return $html;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $idDimensao
     * @param $countRowGlobal
     * @return string
     * @description monta linha com dados vazios para preencher o relatório quando a iniciariva não tem nada cadastrado
     */
    public function htmlMontaLinhaVazia($idDimensao,$countRowGlobal){
        $arrDimensao = $this->getDimensaoById($idDimensao);
        $html = "<tr style='border: solid 1px #000000; padding:5px;'>";
        $html .= "<td style='border: solid 1px #000000; padding:5px;'>".$countRowGlobal." - ".$arrDimensao['dimdsc']."</td>";
        $html .= "<td style='border: solid 1px #000000; padding:5px;'> - </td>";
        $html .= "<td style='border: solid 1px #000000; padding:5px;'> 0 </td>";
        $arrAnos = $this->getAnosCadastradosNasDimensoes();
        for($i = 0; $i< count($arrAnos); $i++){
            $html .= "<td style='border: solid 1px #000000; padding:5px;'> R$00,00 </td>";
        }
        $html .= "<td style='border: solid 1px #000000; padding:5px;'> R$00,00 </td>";
        $html .= "</tr>";
        return $html;
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $dimid
     * @param $countRowAll
     * @return string
     * @description monta tabela com detalhes das iniciativas por dimensão |Dimensão|Iniciativa|Quantidade de Iniciativas Planejamento|{anos}|Total
     */
    public function montaLinhaIniciativasByDimensao($dimid,$countRowAll) {
        $dadosIniciativasDaDimensao = $this->getQuantidadeValorPorDimensao($dimid);//quantidade e valor total da dimensao
        $valorTotal = ($dadosIniciativasDaDimensao['valor_total'] == 0 || $dadosIniciativasDaDimensao['valor_total'] == null)?"R$00,00":"R$".number_format($dadosIniciativasDaDimensao['valor_total'],2,',','.');
        $dbIniciativasPlanejadasDaDimensao = $this->getIniciativasDaDimensaoDaUnidade($dimid);
        $qt_iniciativas = count($dbIniciativasPlanejadasDaDimensao);
        $arrAnos = $this->getAnosCadastradosNasDimensoes();
        $html = '';
        $countRowLocal =0;
        if(isset($dbIniciativasPlanejadasDaDimensao[0])) {
            foreach ($dbIniciativasPlanejadasDaDimensao as $key => $values) {
                $html .= "<tr style='border: solid 1px #000000; padding:5px;'>";
                if ($countRowLocal == 0) {
                    $html .= "<td  style='border: solid 1px #000000; padding:5px;' rowspan='{$qt_iniciativas}'>" . $countRowAll . " - " . $dadosIniciativasDaDimensao['descricao'] . "</td>";
                }
                $html .= "<td  style='border: solid 1px #000000; padding:5px;'>" . $values['descricaoiniciativa'] . "</td>";
                if ($countRowLocal == 0) {
                    $html .= "<td  rowspan='{$qt_iniciativas}' style='border: solid 1px #000000; padding:5px;'>" . $dadosIniciativasDaDimensao['quantidade_iniciativas'] . "</td>";
                }

                for ($i = 0; $i < count($arrAnos); $i++) {
                    if (isset($values['iniid']) && $values['iniid'] != null && $values['iniid'] != '') {
                        $vlIniPorAno = $this->getValorIniciativaPlanejamentoPorAno($values['iniid'], $arrAnos[$i]['ano']);
                        $html .= ($vlIniPorAno['valor_por_ano'] == 0 || $vlIniPorAno == null) ? "<td style='border: solid 1px #000000; padding:5px;'>R$00,00</td>" : "<td style='border: solid 1px #000000; padding:5px;'>R$" . number_format($vlIniPorAno['valor_por_ano'], 2, ',', '.') . "</td>";
                    } else {
                        $html .= "<td style='border: solid 1px #000000; padding:5px;'>R$00,00</td>";
                    }
                    $vlIniPorAno = 0;
                }

                if ($countRowLocal == 0) {
                    $html .= "<td  rowspan='{$qt_iniciativas}' style='border: solid 1px #000000; padding:5px;'>" . $valorTotal . "</td>";
                }
                $html .= "</tr>";
                $countRowLocal++;
            }
        }
        return $html;
    }


    /**
     * @author Leo Kenzley<leo.oliveira@castgroup.com.br>
     * @description Quando não tiver iniciativa, monta uma linha zerada
     */
    public function htmlLinhaIniciativaPorDimensaoVazia($dimensaodesc){
        $arrAnos = $this->getAnosCadastradosNasDimensoes();
        $html =  "<tr>";
        $html .=  "<td>$dimensaodesc<td>";
        foreach($this->getAnosCadastradosNasDimensoes() as $key => $values){
          if($values['ano']!= ''){
              $html .= "<td>R$00,00<td>";
          }
        }
        $html .= "<tr>";
        return $html;
    }

    /**
     * @author Leo Kenzley<leo.oliveira@castgroup.com.br>
     * @param $dimid
     * @return array|mixed|NULL
     * @throws Exception
     * @description retorna os detalhes das iniciativas dadimensão
     */
    public function getIniciativasDaDimensaoDaUnidade($dimid){
        $sql =  "select distinct 
            iniciativa.iniid,
            iniciativa.iniid || ' - ' ||iniciativadescricao.inddsc as descricaoiniciativa,
            dimensao.dimid||' - '||dimensao.dimdsc as dimensaodesc,
            dimensao.dimid,
            dimensao.dimdsc,
            iniciativaplanejamento.inpid,
            iniciativa.intoid,
            iniciativa.intaid,
            iniciativa.iniesfera,
            (select count(*) 
                    from par3.iniciativa_planejamento as inp 
                        where inp.iniid = iniciativa.iniid and inp.dimid = dimensao.dimid and inp.inuid = instrumentounidade.inuid) as qt_iniciativas,
                        
            (select SUM(inp.inpvalor_planejado_total) 
                    from par3.iniciativa_planejamento as inp 
                        where inp.iniid = iniciativa.iniid and inp.dimid = dimensao.dimid and inp.inuid = instrumentounidade.inuid) as valor_iniciativas
            from par3.iniciativa_planejamento as iniciativaplanejamento 
            inner join par3.instrumentounidade as instrumentounidade on instrumentounidade.inuid = iniciativaplanejamento.inuid
            inner join par3.iniciativa as iniciativa on iniciativa.iniid = iniciativaplanejamento.iniid
            inner join par3.iniciativa_descricao as iniciativadescricao on iniciativa.indid = iniciativadescricao.indid
            inner join par3.dimensao as dimensao on dimensao.dimid = iniciativaplanejamento.dimid
            inner join workflow.documento as doc on doc.docid = iniciativaplanejamento.docid
            inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
            where 
            iniciativaplanejamento.dimid = {$dimid}
            and 
            esd.esdid not in ({$this->getNotInEstadoDocumentoIniciativaPlanejamento()}) and 
            instrumentounidade.inuid  = {$this->instrumentoUnidade} 
            order by iniciativa.iniid asc";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley<leo.oliveira@castgroup.com.br>
     * @param $dimensao
     * @return array|bool|mixed|NULL
     * @throws Exception
     * @description retorna os detalhes quantidade e velor total da dimensão
     */
    private function getQuantidadeValorPorDimensao($dimensao){
        $sql =  "select distinct
                    dimensao.dimid,
                    dimensao.dimdsc as descricao,
                    dimensao.dimid||' - '||dimensao.dimdsc as dimdsc,
                    count(iniciativaplanejamento.inpid)	as quantidade_iniciativas,
                    SUM(iniciativaplanejamento.inpvalor_planejado_total) as valor_total
                    from par3.iniciativa_planejamento as iniciativaplanejamento 
                    inner join par3.instrumentounidade as instrumentounidade on instrumentounidade.inuid = iniciativaplanejamento.inuid
                    inner join par3.iniciativa as iniciativa on iniciativa.iniid = iniciativaplanejamento.iniid
                    inner join par3.iniciativa_descricao as iniciativadescricao on iniciativa.indid = iniciativadescricao.indid
                    inner join par3.dimensao as dimensao on dimensao.dimid = iniciativaplanejamento.dimid
                    inner join workflow.documento as doc on doc.docid = iniciativaplanejamento.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    where 
                    iniciativaplanejamento.dimid = {$dimensao}
                    and 
                    iniciativaplanejamento.inpstatus = 'A'
                    and 
                    instrumentounidade.inuid  = {$this->instrumentoUnidade}
                    AND 
                     esd.esdid not in ({$this->getNotInEstadoDocumentoIniciativaPlanejamento()})
                    group by dimensao.dimid";
//        ver($sql);
        try{
            $result = $this->pegaLinha($sql);
            return $result;
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley<leo.oliveira@castgroup.com.br>
     * @param $dimid
     * @return array|bool|mixed|NULL
     * @description retorna os dados da dimensão pelo dimid
     */
    private function getDimensaoById($dimid){
        $sql = "SELECT * FROM par3.dimensao where dimid = {$dimid}";
        return $this->pegaLinha($sql);
    }

    /**
     * @author Leo Kenzley<leo.oliveira@castgroup.com.br>
     * @param $iniid
     * @param $ano
     * @return array|bool|mixed|NULL
     * @throws Exception
     * @description Restorna o valor da iniciativa por ano
     */
    private function getValorIniciativaPlanejamentoPorAno($iniid,$ano){
        $sql = "select 
                SUM((itec.ipiquantidade * itec.ipivalorreferencia)) as valor_por_ano
                from par3.iniciativa_planejamento_item_composicao as itec
                inner join par3.iniciativa_planejamento as inp on inp.inpid = itec.inpid
                inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
                inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                where inp.inuid = {$this->instrumentoUnidade}
                and inp.inpstatus = 'A'
                and itec.ipistatus = 'A'
                and ini.iniid = {$iniid}
                and itec.ipiano = {$ano}
                ";
        try{
            $result = $this->pegaLinha($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley<leo.oliveira@castgroup.com.br>
     * @return string
     * @description rederiza a tabela que mostra os detalhes das iniciavas como dimensão, descricao iniciativa, qtdm anos e total
     */
    public function renderTabelaDescricaoIniciativaPorDimensao(){
        $arrAnos = $this->getAnosCadastradosNasDimensoes();
        $html = "<div class=\"row\" style='padding-bottom: 10px;'>
                        <div class=\"col-md-12\">";
        $html .= '';
        $html .="<table style='border-collapse: collapse; border: 1px solid #000000;'>";
        $html .="<thead>";
        $html .=    "<th>Dimensão</th>";
        $html .=    "<th>Iniciativa</th>";
        $html .=    "<th width=\"50px\">Quantidade de Iniciativas Planejamento</th>";
        $html .= $this->getAnos();
        $html .= "<th>Total</th>";
        $html .= "</thead>";
        $html .= "<tbody>";
        $html .=    $this->htmlTabelaDescricaoIniciativaPorDimensao();
        $html .= "</tbody>";
        $html .= "</table>";
        $html .= "</div></div><br/>";
        return $html;
    }

     /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return array|mixed|NULL
     * @throws Exception
     * @description pega a lista de dimensões conforme formatado no campo select do cadastro de iniciativa no planejamento
     */
    public function getListaDimensoesPadraoPar(){
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $modelInstrumentoUnidade = ($modelInstrumentoUnidade->create($this->instrumentoUnidade));
        $where = $modelInstrumentoUnidade['itrid']? "AND itrid in({$modelInstrumentoUnidade['itrid']})":'';
        $sql = "SELECT dimid as codigo, 
                  (CASE 
                      WHEN itrid = 1 THEN dimcod||' - '||dimdsc||' - '||'Estadual'
                      WHEN itrid = 3 THEN dimcod||' - '||dimdsc||' - '||'Distrital'
                      ELSE dimcod||' - '||dimdsc||' - '||'Municipal'
                    END
                  ) as descricao 
                  FROM par3.dimensao WHERE dimstatus = 'A' {$where}
                  ORDER BY 2";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @throws Exception
     * @description monta o cabecalho da iniciativa
     */
    private function htmlCabecalhoIniciativa($inpid){
        $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'cabecalhoIniciativa.inc';
        $dbCabecalho = $this->getDadosIniciativaPlanejamentoById($inpid); //dados da iniciava
        if(isset($dbCabecalho['inpid'])){
            if(file_exists($arquivo)){
                include($arquivo);
            }else{
                throw new Exception("Arquivo: ".$arquivo." Não encontrado");
            }
        }

    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @return array|bool|mixed|NULL
     * @throws Exception
     * @description retorna os dados para montar o cabecalho da iniciativa
     */
    private function getDadosIniciativaPlanejamentoById($inpid){
                $sql = " 
                select 
                    inp.inpid,
                    ciclo.cicdsc,
                    dim.dimdsc,
                    dim.dimid,
                    tipoatendimento.intaid,
                    ini.iniid||' - '||ind.inddsc as inddsc,
                    (select array_agg(anos.iniano) from par3.iniciativa_planejamento as inipl
                    inner join par3.iniciativa as ini on ini.iniid = inipl.iniid
                    inner join par3.iniciativa_iniciativas_anos as anos on anos.iniid = ini.iniid
                    where inipl.inpid = inp.inpid) as anos,
                    tipoobjeto.intodsc,
                    tipoatendimento.intadsc,
                    modalidade.moddsc,
                    (select array_agg(desdo.desdsc) from par3.iniciativa_planejamento_desdobramentos as desdobramentos
                        inner join par3.iniciativa_desdobramento as des on desdobramentos.desid = des.desid and desdobramentos.inpid = inp.inpid
                        inner join par3.iniciativa_desdobramento as desdo on desdo.desid = des.desid
                    ) as des,
                    ensinoetapa.etadsc,
                    nivel.nivdsc,
                    
                    case  
                        when ini.iniesfera = 'E' then 'Estadual'
                        when ini.iniesfera = 'M' then 'Municipal'
                        when ini.iniesfera = 'T' then 'Todas' end as esfera,
                    (SELECT  
                        array_agg(prog.prgabreviatura||' - '||prog.prgdsc) as programas
                    from par3.iniciativa_planejamento inpl
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_iniciativas_programas as programas on programas.iniid = ini.iniid
                    inner join par3.programa as prog on prog.prgid = programas.prgid
                    where inpl.inpid = inp.inpid) as programas,
                    
                    (select array_agg(area.iarsigla||' - '||area.iardsc) from par3.iniciativa_planejamento as inipl
                    inner join par3.iniciativa as ini on ini.iniid = inipl.iniid
                    inner join par3.iniciativa_iniciativas_areas as areas on areas.iniid = ini.iniid
                    inner join par3.iniciativa_areas as area on area.iarid = areas.iarid
                    where inipl.inpid = inp.inpid) as areasrelacionadas,
                    
                    (select array_agg(proj.prodsc) from par3.iniciativa_planejamento as inipl
                    inner join par3.iniciativa as ini on ini.iniid = inipl.iniid
                    inner join par3.iniciativa_iniciativas_projetos as projetos on projetos.iniid = ini.iniid
                    inner join par3.projeto as proj on proj.proid = projetos.proid
                    where inipl.inpid = inp.inpid) as projetos
                    
                    from par3.iniciativa_planejamento as inp
                    inner join par3.instrumentounidade as inu on inp.inuid = inu.inuid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_descricao as ind on ini.indid = ind.indid
                    inner join par3.dimensao as dim on dim.dimid = inp.dimid 
                    inner join par3.ciclo_par as ciclo on ciclo.cicid = ini.cicid
                    inner join par3.iniciativa_iniciativas_anos as anos on ini.iniid =  anos.iniid
                    inner join par3.iniciativa_tipos_objeto  as tipoobjeto on tipoobjeto.intoid = ini.intoid
                    inner join par3.iniciativa_tipos_atendimento as tipoatendimento on tipoatendimento.intaid = ini.intaid
                    inner join par3.ensino_modalidade as ensinomodalidade on inp.modid = ensinomodalidade.modid
                    inner join par3.modalidade as modalidade on modalidade.modid = ensinomodalidade.modid
                    inner join par3.ensino_etapa as ensinoetapa on ensinoetapa.etaid = inp.etaid
                    inner join par3.ensino_nivel as nivel on nivel.nivid = ensinoetapa.nivid
                    inner join par3.iniciativa_planejamento_desdobramentos as desdobramentos on desdobramentos.inpid = inp.inpid
                    inner join par3.iniciativa_desdobramento as des on des.desid = desdobramentos.desid
                    inner join par3.iniciativa_iniciativas_programas as programas on programas.iniid = ini.iniid
                    inner join par3.programa as pro on pro.prgid = programas.prgid
                    inner join par3.iniciativa_iniciativas_areas as areas on areas.iniid = ini.iniid
                    inner join par3.iniciativa_areas as area on area.iarid = areas.iarid
                    where inp.inpid = {$inpid}
                    group by 
                    inp.inpid,ciclo.cicdsc,dim.dimdsc,ind.inddsc,tipoobjeto.intodsc,tipoatendimento.intadsc,modalidade.moddsc,ensinoetapa.etadsc,
                    nivel.nivdsc,ini.iniesfera,ini.iniid,dim.dimid,tipoatendimento.intaid";
               // ver($sql);

                try{
                    $result = $this->pegaLinha($sql);
                    $result['dimesoespadraopar'] = $this->getDescricaoDimensaoByIdDimensao($result['dimid']);
                    return $result;
                }catch (Exception $e){
                    throw new Exception($e->getCode()." - ".$e->getMessage());
                }


    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $dimid
     * @return string
     * @description retorna os dados da dimensão sem ser o formado do par3
     */
    private  function getDescricaoDimensaoByIdDimensao($dimid)
    {
        $desc = '';
        $arrDimesoes = $this->getListaDimensoesPadraoPar();
        foreach($arrDimesoes as $key => $value){
            if($value['codigo'] == $dimid){
                $desc = $value['descricao'];
            }
        }
        return $desc;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description Esta função junta o cabecalho da iniciativa os detalhes de itens de composição
     *
     */
    public function renderDetalhaIniciativasPorDimensao(){
        $arrayIniciativasDaDimensao = $this->getDimensaoCadastradaUnidade();
        if(isset($arrayIniciativasDaDimensao[0])){
            foreach($arrayIniciativasDaDimensao as $key => $value){
                $this->htmlDetalhaIniciativaPorDimensao($value['dimid']);
            }
        }

    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @monta
     * @param $dimid
     * @throws Exception
     * @description esta função carrega os dados das iniciativas por dimensão
     *  - monta o cabecalho da iniciativa
     *  - monta os itens de composição por ano e suas iniciarivas
     *  -
     */
    private function htmlDetalhaIniciativaPorDimensao($dimid)
    {
        $arrIniciativaDaDimensao = $this->getIniciativasDaDimensaoDaUnidade($dimid);
        //ver($arrIniciativaDaDimensao);
        $arrAnos = $this->getAnosCadastradosNasDimensoes();
        echo '<div class="row"><div class="pcol-md-12">';
        if(isset($arrIniciativaDaDimensao[0])){
            foreach ($arrIniciativaDaDimensao as $key => $valueI) { //lista as iniciativas da dimensão
                if ($valueI['iniesfera'] == 'T' || $valueI['iniesfera'] == 'M') { //SE FOR DO TIPO MUNICIPAL
                    $this->htmlCabecalhoIniciativa($valueI['inpid']); //CABEÇALHO COM OS DADOS DA INICIATIVA
                    foreach ($arrAnos as $key => $valueA) { //lista os anos cadastrados para a unidade
                        //OBJETO do tipo FORMAÇÃO
                        if($valueI['intoid'] == 2){
                            if($valueI['intaid'] == 2){
                                //se além de ser do tipo de objeto formação, e o tipo de atendimento for por escola
                                $this->htmlRowItemComposicaoPorAnoEscolas($valueI,$valueA['ano']);
                            }elseif($valueI['intaid'] == 1){ //se for global
                                //dados dos Itens de composicao
                                $dadosItensComposicao = $this->getListaItensComposicaoTipoObjetoFormacaoGlobalByIniciativaPlanejamentoAno($valueI['inpid'],$valueA['ano']);
                                if(count($dadosItensComposicao[0]) > 0):
                                    $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoGlobal.inc';
                                    if(file_exists($arquivo)){
                                        include($arquivo);
                                    }else{
                                        throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                                    }
                                endif;
                            }
                        }
                        //CASO SEJA DIFERENTE DE OBRA E DE FORMAÇÃO
                        elseif ($valueI['intoid'] != 2 && $valueI['intoid'] != 1 ){
                           // if($valueI['intaid'] == 2) { //SE O TIPO DE ATENDIMENTO FOR DO TIPO POR ESCOLA
                                //$arrItensComposicaoDaIniciativaAno = $this->getItensComposicaoByIniciativaPlanejamentoAno($valueI['inpid'],$valueA['ano']);
                                $this->htmlItensComposicaoPorEscolaGeneric($valueI,$valueA['ano']);
                            //}

                        }

                    }

                    //TIPO DE OBJETO


                    //OBRA
                    $this->htmlDadosItemComposicaoObra($valueI);

                    //FORMAÇÃO
                    $this->htmlFormacaoGlobalInformacoesGerais($valueI);
                    $this->htmlDetalhamentoDasEtapas($valueI);
                    $this->htmlAtividadesDasEtapas($valueI);
                    $this->htmlInsumoDaIniciativa($valueI);
                    $this->htmlGeneric($valueI);
                    $this->htmlValorTotalDaIniciativa($valueI);

                    //DEMAIS

                }
                //SE a esfera for estadual ou Distrital
                elseif($valueI['iniesfera'] == 'T' || $valueI['iniesfera'] == 'E' || $valueI['iniesfera'] == 'D') {
                    //CABECALHO DAS INICIATIVAS
                    $this->htmlCabecalhoIniciativa($valueI['inpid']);
                    //Se Tipo de objeto for FORMAÇÃO
                    if($valueI['intoid'] == 2){
                        //anos cadastrados
                        foreach ($arrAnos as $key => $valueA) {
                            if($valueI['intaid'] == 2){ //SE a formação for por escola
                                $this->htmlRowItemComposicaoPorAnoEscolasMunicipio($valueI,$valueA['ano']);
                            }elseif($valueI['intaid'] == 1){ //se for global
                                //dados dos Itens de composicao
                                $dadosItensComposicao = $this->getListaItensComposicaoTipoObjetoFormacaoGlobalByIniciativaPlanejamentoAno($valueI['inpid'],$valueA['ano']);
                                if(count($dadosItensComposicao[0]) > 0):
                                    $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoGlobal.inc';
                                    if(file_exists($arquivo)){
                                        include($arquivo);
                                    }else{
                                        throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                                    }
                                endif;
                            }
                        }
                    }

                    //OBRA
                    $this->htmlDadosItemComposicaoObra($valueI);

                    //FORMAÇÃO
                    $this->htmlFormacaoGlobalInformacoesGerais($valueI);
                    $this->htmlDetalhamentoDasEtapas($valueI);
                    $this->htmlAtividadesDasEtapas($valueI);
                    $this->htmlInsumoDaIniciativa($valueI);
                    $this->htmlGeneric($valueI);
                    $this->htmlValorTotalDaIniciativa($valueI);
                }
            }
        }
        echo "</div></div>";

    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @param $ano
     * @throws Exception
     * @description Este método monta lista de Escolas beneficiadas com os itens de composição para os tipos de objetos genéricos
     */
    private function htmlItensComposicaoPorEscolaGeneric($inp = array(),$ano){
        //Se o tipo de objeto não for do tipo OBRA e FORMAÇÃO
        if($inp['intoid'] != 1 && $inp['intoid'] != 2){
            $dbItensComposicaoDaIniciativa = $this->getItensDeComposicaoGenericByIniciativaPlanejamento($inp);
            if(isset($dbItensComposicaoDaIniciativa[0])){
                foreach($dbItensComposicaoDaIniciativa as $key => $value){
                    $dbItemComposicao = $this->createIniciativaItenDeComposicao($inp['inpid'],$ano,$value['itcid']);
                    $dbItensComposicaoPorEscolas = $this->getDadosItensComposicaoByIniciativaPlanejamentoAnoItemComposicao($inp,$ano,$value['itcid']);
                    if(isset($dbItensComposicaoPorEscolas[0])){
                        $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoGenericoEscola.inc';
                        if(file_exists($arquivo)){
                            include($arquivo);
                        }else{
                            throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                        }
                    }

                }

            }
        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @param $ano
     * @param $itemComposicao
     * @return array|mixed|NULL
     * @throws Exception
     * @description Esta função retorna os itens de composição por ano dos tipos de objetos genéricos
     */
    private function getDadosItensComposicaoByIniciativaPlanejamentoAnoItemComposicao($inp = array(),$ano,$itemComposicao){
        $sql = "select * from par3.iniciativa_planejamento as inp
                    inner join par3.iniciativa_planejamento_item_composicao as iniplaitecom on iniplaitecom.inpid = inp.inpid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitecomgru on iniitecomgru.iigid = iniplaitecom.iigid
                    inner join par3.itenscomposicao as ite on ite.itcid = iniitecomgru.itcid
                    inner join par3.unidade_medida as unid on unid.uniid = ite.uniid
                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplaitecomesc on iniplaitecomesc.ipiid = iniplaitecom.ipiid
                    inner join par3.escola as esc on esc.escid = iniplaitecomesc.escid
                    where 
                    iniplaitecom.ipiano = {$ano} and 
                    inp.inpid = {$inp['inpid']} and 
                    ite.itcid = {$itemComposicao} and
                    iniplaitecomesc.ipequantidade > 0
                    order by iniplaitecom.ipiano,ite.itcdsc,esc.escnome asc";
        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @return array|mixed|NULL
     * @throws Exception
     * @description Este Método retorna os itens de composição cadastrados na iniciativa do planejamento
     */
    private function getItensDeComposicaoGenericByIniciativaPlanejamento($inp = array()){
        $sql = "select DISTINCT ite.itcid from par3.itenscomposicao as ite
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitegro on iniitegro.itcid = ite.itcid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitecom on iniplanitecom.iigid = iniitegro.iigid
                    inner join par3.iniciativa_planejamento as inp on inp.inpid = iniplanitecom.inpid
                    where 
                    inp.inpid = {$inp['inpid']} 
                    order by ite.itcid asc";

        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }

    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @return array|mixed|NULL
     * @throws Exception
     * @description Este método retorna os itens de composição dos tipos genéricos
     */
    private function getItensComposicaoGenericGlobal($inp = array()){
        $sql = "select * from par3.iniciativa_planejamento as inp
                    inner join par3.iniciativa_planejamento_item_composicao as inpplaitecom on inpplaitecom.inpid = inp.inpid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_descricao as inid on inid.indid = ini.indid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitegru on iniitegru.iigid = inpplaitecom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitegru.itcid
                    inner join par3.unidade_medida as unid on unid.uniid = intcom.uniid
                    where 
                    inp.inpid = {$inp['inpid']}
                    order by ipiano, inid.inddsc asc ";
        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }
    }


    private function htmlGeneric($inp = array()){

        if($inp['iniesfera'] == 'T' || $inp['iniesfera'] == 'M'):
        //Se tipo de objeto for diferente de Formação e obra

        //array de tipos de objetos não genericos FORMAÇÃO|OBRA
        $arrayNotGeneric = array(1,2);

        if(!in_array($inp['intoid'],$arrayNotGeneric)){
            //se o tipo de atendimento for GLOBAL
            if($inp['intaid'] == 1){
                $dadosItensComposicao = $this->getItensComposicaoGenericGlobal($inp);
                if(isset($dadosItensComposicao[0])){
                    $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoGenericoGlobal.inc';
                    if(file_exists($arquivo)){
                        include($arquivo);
                    }else{
                        throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                    }
                }
            }
            //Se tipo objeto por ESCOLA
            elseif ($inp['intaid'] == 2){

            }
        }
        endif;
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @throws Exception
     * @description Esta função retorna lista de itens de composição do tipo obra
     */
    private function htmlDadosItemComposicaoObra($inp = array()){
        //Se o tipo de objeto for obra
        if($inp['intoid'] == 1){
            $dadosIntemComposicaoObras = $this->getDadosItemComposicaoObra($inp);
            if(isset($dadosIntemComposicaoObras[0])){
                $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoObra.inc';
                if(file_exists($arquivo)){
                    include($arquivo);
                }else{
                    throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                }
            }
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @param $obrano
     * @return array|mixed|NULL
     * @throws Exception
     * @description Retorna os dados dos itens de composição que são do tipo Obra
     */
    private function getDadosItemComposicaoObra($inp =  array()){
        $sql = "select * from par3.obra as obra
                    inner join par3.iniciativa_planejamento as inp on obra.inpid = inp.inpid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_tipos_objeto as initipobj on ini.intoid = initipobj.intoid
                    inner join par3.obra_tipo as obrtip on obrtip.otpid = obra.otpid
                    inner join par3.obra_categoria as obrcat on obrcat.octid = obrtip.octid
                    inner join workflow.documento as documento on documento.docid = inp.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = documento.esdid
                where 
                inp.inpid = {$inp['inpid']}  and 
                 esd.esdid not in ({$this->getNotInEstadoDocumentoIniciativaPlanejamento()}) and
                obra.obrstatus = 'A'
                order by obrano, obrdsc asc";
        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }
    }



    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @param $ano
     * @description Esta função monta as linhas dos intens de composição com seus detalhes quando for do tipo formação
     */
    private function htmlRowItemComposicaoPorAnoEscolas($inp = array(),$ano){
        $arrItensComposicaoDaIniciativaAno = $this->getItensComposicaoByIniciativaPlanejamentoAno($inp['inpid'],$ano);
        if(isset($arrItensComposicaoDaIniciativaAno[0])){
            foreach($arrItensComposicaoDaIniciativaAno as $key => $value){
                $dbItemComposicao = $this->createIniciativaItenDeComposicao($inp['inpid'],$ano,$value['itcid']);
                $dbItensComposicaoPorEscolas = $this->getItensDeComposicaoPorEscolaFormacao($inp['inpid'],$ano,$dbItemComposicao['itcid']);
                //$dbValorTotalDaIniciativa = $this->getValorTotalDaIniciativa($inp['inpid']);
                $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoEscola.inc';
                if(file_exists($arquivo)){
                    include($arquivo);
                }else{
                    throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                }
            }

        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @param $ano
     * @description Esta função monta as linhas dos intens de composição com seus detalhes quando for do tipo formação
     * e atendimento for por escola
     */
    private function htmlRowItemComposicaoPorAnoEscolasMunicipio($inp = array(),$ano){
        $arrItensComposicaoDaIniciativaAno = $this->getItensComposicaoByIniciativaPlanejamentoAno($inp['inpid'],$ano);
       //CARREGA TODAS AS Iniciativas dos itens de composição po ano
        if(isset($arrItensComposicaoDaIniciativaAno[0])){
            foreach($arrItensComposicaoDaIniciativaAno as $key => $value){
                //cria lista dos itens de composição por ano
                $dbItemComposicao = $this->createIniciativaItenDeComposicao($inp['inpid'],$ano,$value['itcid']);
                //pega lista de municípios que tem este item de composição
                $dbListaDeEscolasMunicipio = $this->getMunicipiosQuePossuemItemComposicaoByIniciativaPlanejamentoAnoMunicipio($inp,$ano);
                if(isset($dbListaDeEscolasMunicipio)){
                    foreach($dbListaDeEscolasMunicipio as $key => $valueMunics){
                        $dbListaEscolasValores = $this->getListaEscolaValoresPorMunicipioItensComposicao($inp,$ano,$value['itcid'],$valueMunics['muncod']);
                        if(isset($dbListaEscolasValores[0])){
                            $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoEscolaEstadual.inc';
                            if(file_exists($arquivo)){
                                include($arquivo);
                            }else{
                                throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                            }

                        }
                    }
                }
            }

        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @param $ano
     * @param $itemComposicao
     * @return array|mixed|NULL
     * @throws Exception
     * @description Este método pega a lista de escolas que possuem tal item de composicao no respectivo ano
     */
    private function getListaEscolaValoresPorMunicipioItensComposicao($inp = array(),$ano,$itemComposicao,$muncod){
        $sql ="select *
                    from par3.iniciativa_planejamento as inp
                    inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitemcom on iniplanitemcom.inpid = inp.inpid
                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplanitemcomesc on iniplanitemcomesc.ipiid = iniplanitemcom.ipiid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitenscomposgrup on iniitenscomposgrup.iigid = iniplanitemcom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitenscomposgrup.itcid
                    inner join par3.escola as escola on escola.escid = iniplanitemcomesc.escid
                    inner join par3.itenscomposicao_detalhamento as intenscomdet on intenscomdet.itcid = intcom.itcid
                    left join territorios.municipio as munic on munic.muncod = escola.muncod
                    where 
                    iniplanitemcom.ipiano = {$ano} and
                    inp.inpid = {$inp['inpid']}                    
                    and 
                    intcom.itcid = {$itemComposicao}
                    and
                    munic.muncod = '{$muncod}'";
        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }
    }
    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @param $ano
     * @return array|mixed|NULL
     * @throws Exception
     * @description Este Método pega os municípios que possuem escolas com o item de composicao
     */
    private function getMunicipiosQuePossuemItemComposicaoByIniciativaPlanejamentoAnoMunicipio($inp = array(), $ano){
        $sql = "select distinct munic.*
                    from par3.iniciativa_planejamento as inp
                    inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitemcom on iniplanitemcom.inpid = inp.inpid
                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplanitemcomesc on iniplanitemcomesc.ipiid = iniplanitemcom.ipiid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitenscomposgrup on iniitenscomposgrup.iigid = iniplanitemcom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitenscomposgrup.itcid
                    inner join par3.escola as escola on escola.escid = iniplanitemcomesc.escid
                    inner join par3.itenscomposicao_detalhamento as intenscomdet on intenscomdet.itcid = intcom.itcid
                    left join territorios.municipio as munic on munic.muncod = escola.muncod
                    where 
                    iniplanitemcom.ipiano = $ano and
                    inp.inpid = {$inp['inpid']} ";
        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @param $ano
     * @param $itenComposicao
     * @description carrega os dados do item de composição
     */
    private function createIniciativaItenDeComposicao($inpid,$ano,$itenComposicao){
        $sql = "select *,
 (select sum(ipequantidade)
                    from par3.iniciativa_planejamento as inp
                    inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitemcom on iniplanitemcom.inpid = inp.inpid
                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplanitemcomesc on iniplanitemcomesc.ipiid = iniplanitemcom.ipiid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitenscomposgrup on iniitenscomposgrup.iigid = iniplanitemcom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitenscomposgrup.itcid
                    inner join par3.escola as escola on escola.escid = iniplanitemcomesc.escid
                    inner join par3.itenscomposicao_detalhamento as intenscomdet on intenscomdet.itcid = intcom.itcid
                    where 
                    iniplanitemcom.ipiano = $ano and
                    inp.inpid = {$inpid} and 
                    iniplanitemcomesc.ipequantidade > 0 
) as quantidade_por_escola
 from par3.itenscomposicao as ite
                inner join par3.iniciativa_itenscomposicao_grupo as iniitegro on iniitegro.itcid = ite.itcid
                inner join par3.iniciativa_planejamento_item_composicao as iniplanitecom on iniplanitecom.iigid = iniitegro.iigid
                inner join par3.iniciativa_planejamento as inp on inp.inpid = iniplanitecom.inpid
                inner join par3.unidade_medida as und on und.uniid = ite.uniid
                where 
                inp.inpid = {$inpid} and
                iniplanitecom.ipiano = {$ano} and 
                ite.itcid = {$itenComposicao}
                order by ite.itcdsc asc";
        try{
            return $this->pegaLinha($sql);
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ". $e->getMessage());
        }
    }





    /** ?
     * @param $inpid
     * @param $ano
     * @param $itemComposicao
     * @return array|mixed|NULL
     * @throws Exception
     */
    private function getItensDeComposicaoPorEscolaFormacao($inpid,$ano,$itemComposicao){
        $sql = "select *
                    from par3.iniciativa_planejamento as inp
                    inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitemcom on iniplanitemcom.inpid = inp.inpid
                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplanitemcomesc on iniplanitemcomesc.ipiid = iniplanitemcom.ipiid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitenscomposgrup on iniitenscomposgrup.iigid = iniplanitemcom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitenscomposgrup.itcid
                    inner join par3.escola as escola on escola.escid = iniplanitemcomesc.escid
                    inner join par3.itenscomposicao_detalhamento as intenscomdet on intenscomdet.itcid = intcom.itcid
                    where 
                    iniplanitemcom.ipiano = {$ano} and
                    inp.inpid = {$inpid} and 
                    intcom.itcid = {$itemComposicao}";
        //ver($sql);
        try{
            return $this->carregar($sql);
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    /** ?
     * @param $inpid
     * @param $ano
     */
//    private function getItensDeComposicaoPorEscolaFormacaoEstadual($inpid,$ano){
//        $sql = "select *
//                    from par3.iniciativa_planejamento as inp
//                    inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
//                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
//                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitemcom on iniplanitemcom.inpid = inp.inpid
//                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplanitemcomesc on iniplanitemcomesc.ipiid = iniplanitemcom.ipiid
//                    inner join par3.iniciativa_itenscomposicao_grupo as iniitenscomposgrup on iniitenscomposgrup.iigid = iniplanitemcom.iigid
//                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitenscomposgrup.itcid
//                    inner join par3.escola as escola on escola.escid = iniplanitemcomesc.escid
//                    inner join par3.itenscomposicao_detalhamento as intenscomdet on intenscomdet.itcid = intcom.itcid
//                    left join territorios.municipio as munic on munic.muncod = escola.muncod
//                    where
//                    iniplanitemcom.ipiano = {$ano} and
//                    inp.inpid = $inpid ";
//    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @param $ano
     * @return array|mixed|NULL
     * @throws Exception
     * @description Pega os itens de composição cadastrados na iniciativa planejada e no seguinte ano
     */
    private function getItensComposicaoByIniciativaPlanejamentoAno($inpid,$ano){
        $sql = "select * from par3.itenscomposicao as ite
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitegro on iniitegro.itcid = ite.itcid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitecom on iniplanitecom.iigid = iniitegro.iigid
                    inner join par3.iniciativa_planejamento as inp on inp.inpid = iniplanitecom.inpid
                    where 
                    inp.inpid = {$inpid} and
                    iniplanitecom.ipiano = {$ano} 
                    order by ite.itcdsc asc";
        try{
            return $this->carregar($sql);
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    private function getDadosLinhaTipoFormacaoPorEscola(){

    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @return array|mixed|NULL
     * @throws Exception
     * @description pega todos os itens de composição ligados a iniciativa
     */
    private function getItensComposicaoDaIniciativaByIniciativaPlanejamento($inpid){
        $sql = "select DISTINCT ite.itcid, ite.itcdsc from par3.itenscomposicao as ite
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitegro on iniitegro.itcid = ite.itcid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitecom on iniplanitecom.iigid = iniitegro.iigid
                    where iniplanitecom.inpid = {$inpid}
                    order by ite.itcdsc asc";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch (Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @description Esta função retorna o valor total planejado da iniciativa
     */
    private function htmlValorTotalDaIniciativa($inpid = array()){
        $dbValorTotalIniciativa = $this->getValorTotalDaIniciativa($inpid);
        $valor = ($dbValorTotalIniciativa['inpvalor_planejado_total'] == 0 || $dbValorTotalIniciativa['inpvalor_planejado_total'] == null || $dbValorTotalIniciativa['inpvalor_planejado_total'] == '')?
            " 00,00": number_format($dbValorTotalIniciativa['inpvalor_planejado_total'],2,',','.');
        //$dados = array("valor_total_iniciativa" => $valor );
        $this->insertArquivoRelatorio("valorTotalIniciativa",
            array(
                'valor_total_iniciativa' => $valor,
        ));
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description retorna o valor total da iniciativa
     *  - Soma de (ipivalorreferencia * ipiquantidade)
     * @return $result['inpvalor_planejado_total'];
     */
    private function getValorTotalDaIniciativa($inp){
        $sql = '';
        //FORMAÇÃO
        if($inp['intoid'] == 2){// se o tipo de objeto for 2 FORMAÇÃO
            if($inp['intaid'] == 2){ //se o tipo de objeto for formação e atendimento por escola
                $sql = "select distinct inp.inpvalor_planejado_total
                    from par3.iniciativa_planejamento as inp
                    inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitemcom on iniplanitemcom.inpid = inp.inpid
                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplanitemcomesc on iniplanitemcomesc.ipiid = iniplanitemcom.ipiid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitenscomposgrup on iniitenscomposgrup.iigid = iniplanitemcom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitenscomposgrup.itcid
                    inner join par3.escola as escola on escola.escid = iniplanitemcomesc.escid
                    inner join par3.itenscomposicao_detalhamento as intenscomdet on intenscomdet.itcid = intcom.itcid
                    inner join workflow.documento as doc on doc.docid = inp.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    where 
                    inp.inpid =  {$inp['inpid']}";
            }else if($inp['intaid'] == 1){ //se o tipo de objeto for formação e atendimento global
                $sql = "select distinct inp.inpvalor_planejado_total from par3.iniciativa_planejamento as inp
                            inner join par3.iniciativa_planejamento_item_composicao as inpplaitecom on inpplaitecom.inpid = inp.inpid
                            inner join par3.iniciativa_itenscomposicao_grupo as iniitegru on iniitegru.iigid = inpplaitecom.iigid
                            inner join par3.itenscomposicao as intcom on intcom.itcid = iniitegru.itcid
                            inner join par3.unidade_medida as unid on unid.uniid = intcom.uniid
                            inner join workflow.documento as doc on doc.docid = inp.docid
                            inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                            where 
                            inp.inpid = {$inp['inpid']}  ";
            }
        }

        // se for do tipo obra
        else if($inp['intoid'] == 1){
            $sql =  "select SUM(obrvalor) as inpvalor_planejado_total from par3.obra as obra
                    inner join par3.iniciativa_planejamento as inp on obra.inpid = inp.inpid
                    inner join workflow.documento as doc on doc.docid = inp.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    where 
                    inp.inpid = {$inp['inpid']}  and obra.obrstatus = 'A'";
        }

        //generico
        elseif($inp['intoid'] != 1 && $inp['intoid'] != 2){//remover
            if($inp['intaid'] == 1){;//se for global
                $sql = "select 
                      SUM(inpplaitecom.ipivalorreferencia * inpplaitecom.ipiquantidade) as inpvalor_planejado_total
                          from par3.iniciativa_planejamento as inp
				left join par3.iniciativa_planejamento_item_composicao as inpplaitecom on inpplaitecom.inpid = inp.inpid
                left join par3.iniciativa_itenscomposicao_grupo as iniitegru on iniitegru.iigid = inpplaitecom.iigid
                left join par3.itenscomposicao as intcom on intcom.itcid = iniitegru.itcid
                left join par3.unidade_medida as unid on unid.uniid = intcom.uniid
                inner join workflow.documento as doc on doc.docid = inp.docid
                inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                where inp.inpid =  {$inp['inpid']}";
            }else if($inp['intaid'] == 2){ //se for por escola
                $sql = "select distinct inp.inpvalor_planejado_total
                    from par3.iniciativa_planejamento as inp
                    inner join par3.instrumentounidade as inu on inu.inuid = inp.inuid
                    inner join par3.iniciativa as ini on ini.iniid = inp.iniid
                    inner join par3.iniciativa_planejamento_item_composicao as iniplanitemcom on iniplanitemcom.inpid = inp.inpid
                    inner join par3.iniciativa_planejamento_item_composicao_escola as iniplanitemcomesc on iniplanitemcomesc.ipiid = iniplanitemcom.ipiid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitenscomposgrup on iniitenscomposgrup.iigid = iniplanitemcom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitenscomposgrup.itcid
                    inner join par3.escola as escola on escola.escid = iniplanitemcomesc.escid
                    inner join par3.itenscomposicao_detalhamento as intenscomdet on intenscomdet.itcid = intcom.itcid
                    inner join workflow.documento as doc on doc.docid = inp.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    where 
                    inp.inpid =  {$inp['inpid']}";
            }

        }
        try{
            $result = $this->pegaLinha($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inpid
     * @return array|mixed|NULL
     * @throws Exception
     * @description Esta função retorna uma lista de Itens de composição quando o tipo de objeto é FORMAÇÃO e o tipo de aten
     * dimento é GLOBAL
     */
    private function getListaItensComposicaoTipoObjetoFormacaoGlobalByIniciativaPlanejamentoAno($inpid,$ano){
        $sql = "select * from par3.iniciativa_planejamento as inp
                    inner join par3.iniciativa_planejamento_item_composicao as inpplaitecom on inpplaitecom.inpid = inp.inpid
                    inner join par3.iniciativa_itenscomposicao_grupo as iniitegru on iniitegru.iigid = inpplaitecom.iigid
                    inner join par3.itenscomposicao as intcom on intcom.itcid = iniitegru.itcid
                    inner join par3.unidade_medida as unid on unid.uniid = intcom.uniid
                    inner join workflow.documento as doc on doc.docid = inp.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    where 
                    inp.inpid = {$inpid} and
                    ipiano = {$ano} and 
                     esd.esdid not in ({$this->getNotInEstadoDocumentoIniciativaPlanejamento()})
                    order by ipiano,itcdsc asc";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @throws Exception
     * @description Esta função cria a linha informações gerais quando o ipo de objeto é formação e atendimento global
     */
    private function htmlFormacaoGlobalInformacoesGerais($inp = array()){
        //se tipo de objeto = formação e atendimento global
        if($inp['intoid'] == 2 && $inp['intaid'] == 1):
            $dadosInfomacoesGerais = $this->getInformacoesGerais($inp);
        //ver($dadosInfomacoesGerais);
            if(isset($dadosInfomacoesGerais[0]) || isset($dadosInfomacoesGerais['qt_professores'])):
                $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoGlobalInformacoesGerais.inc';
                if(file_exists($arquivo)){
                    include($arquivo);
                }else{
                    throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                }
            endif;
        endif;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @description esta função pega a quantidade de professores,orientadores ... cadastrados na iniciativa planejada nos itens de composição
     */
    private function getInformacoesGerais($inp = array()){
        $sql =  "select 
                            DISTINCT
                            (select 
                            count(ipifprofessores)
                            from par3.iniciativa_planejamento_informacoesiniciativa
                            where 
                            iniciativa_planejamento_informacoesiniciativa.inpid = {$inp['inpid']}  and ipifprofessores <> '') as qt_professores,
                            (select 
                            count(ipiforientadores)
                            from par3.iniciativa_planejamento_informacoesiniciativa
                            where 
                            iniciativa_planejamento_informacoesiniciativa.inpid = {$inp['inpid']}  and ipiforientadores <> '' ) as qt_ipiforientadores,
                            (select 
                            count(ipifformadores)
                            from par3.iniciativa_planejamento_informacoesiniciativa
                            where 
                            iniciativa_planejamento_informacoesiniciativa.inpid = {$inp['inpid']}  and ipifformadores <> ''  ) as qt_ipifformadores,
                            (select 
                            count(ipiftutores) 
                            from par3.iniciativa_planejamento_informacoesiniciativa
                            where 
                            iniciativa_planejamento_informacoesiniciativa.inpid = {$inp['inpid']}  and ipiftutores <> ''  ) as qt_ipiftutores,
                            (select 
                            count(ipifpreparadores) 
                            from par3.iniciativa_planejamento_informacoesiniciativa
                            where 
                            iniciativa_planejamento_informacoesiniciativa.inpid = {$inp['inpid']} and ipifpreparadores <> '' ) as qt_ipifpreparadores,
                            (select 
                            count(ipifsupervisores) 
                            from par3.iniciativa_planejamento_informacoesiniciativa
                            where 
                            iniciativa_planejamento_informacoesiniciativa.inpid = {$inp['inpid']}  and ipifsupervisores <> '' ) as qt_ipifsupervisores
                            from par3.iniciativa_planejamento_informacoesiniciativa as iniplaninf
                            where
                            iniplaninf.inpid = {$inp['inpid']}";
        try{
            $result = $this->pegaLinha($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @throws Exception
     * @description Esta função retorna a tabela de detalhamento da etapa quando tipo Formacao atendimento Global
     */
    private function htmlDetalhamentoDasEtapas($inp = array()){
        if($inp['intoid'] == 2 && $inp['intaid'] == 1):
        $dadosDetalhamentoEtapas = $this->getDetalhamentoEtapas($inp);
        if(isset($dadosDetalhamentoEtapas[0])):
            $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoGlobalDetalhamentoDasEtapas.inc';
            if(file_exists($arquivo)){
                include($arquivo);
            }else{
                throw new Exception("Arquivo: ".$arquivo." Não encontrado");
            }
            endif;
        endif;
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @return array|mixed|NULL
     * @throws Exception
     * @description Esta função retorna os dados do detalhamento da etapa
     */
    private function getDetalhamentoEtapas($inp = array()){
        $sql = "select * from par3.iniciativa_planejamento_detalheetapa where inpid = {$inp['inpid']} ";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @throws Exception
     * @description Esta função renderiza a tabela de Atividades das etapas quando tipo Formacao atendimento Global
     */
    private function htmlAtividadesDasEtapas($inp = array()){
        if($inp['intoid'] == 2 && $inp['intaid'] == 1):
            $dadosDasAtividadesDasEtapas = $this->getAtividadesDasEtapas($inp);
            if(isset($dadosDasAtividadesDasEtapas[0])):
                $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoGlobalAtividadesDasEtapas.inc';
                if(file_exists($arquivo)){
                    include($arquivo);
                }else{
                    throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                }
            endif;
        endif;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @return array|mixed|NULL
     * @throws Exception
     * @description Esta função retorna os dados das Atividades das Etapas da iniciativa planejada
     */
    private function getAtividadesDasEtapas($inp = array()){
        $sql = "select * from par3.iniciativa_planejamento_atividadeetapa iniplaativ
                inner join par3.iniciativa_planejamento_detalheetapa as inipladet on inipladet.ipidid = iniplaativ.ipidid
                where inipladet.inpid = {$inp['inpid']} ";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @throws Exception
     * @description Esta função renderiza a tabela de Insumos da Iniciativa quando tipo Formacao atendimento Global
     */
    private function htmlInsumoDaIniciativa($inp = array()){
        if($inp['intoid'] == 2 && $inp['intaid'] == 1):
            $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.'tipoObjetoFormacaoGlobalInsumosDaIniciativa.inc';
            $dadosInsumosDaIniciativa = $this->getDadosInsumosDaIniciativa($inp);
                if(isset($dadosInsumosDaIniciativa[0])):
                    if(file_exists($arquivo)){
                        include($arquivo);
                    }else{
                        throw new Exception("Arquivo: ".$arquivo." Não encontrado");
                    }
                 endif;
            endif;

    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $inp
     * @return array|mixed|NULL
     * @throws Exception
     * @description Esta função retorna os dados de Insumos da Iniciativa quando tipo Formacao atendimento Global
     */
    private function getDadosInsumosDaIniciativa($inp = array()){
      $sql =  "select * from par3.iniciativa_planejamento_insumosiniciativa where inpid = {$inp['inpid']}";
        try{
            $result = $this->carregar($sql);
            return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description Insere o arquivo
     * @param $arquivo arquivo .inc que está na pasta simec\par3\modulos\relatorio\planejamentoIniciativasFinalizadas
     */
    private function insertArquivoRelatorio($arquivo,$dados = array()){
        $arquivo = '..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR.'modulos'.DIRECTORY_SEPARATOR.'relatorio'.DIRECTORY_SEPARATOR.'planejamentoIniciativasFinalizadas'.DIRECTORY_SEPARATOR.$arquivo.'.inc';
        $valor = $dados['valor_total_iniciativa'];
        if(file_exists($arquivo)){
            include($arquivo);
        }else{
            throw new Exception("Arquivo: {$arquivo} não encontrado");
        }
    }

    /**
     * @return string
     */
     public function getTituloParUnidade(){
        $modelInu = new Par3_Model_InstrumentoUnidade();
        $unidade = $modelInu->create($this->instrumentoUnidade);
        $text = "";
        if($unidade['itrid'] == 1 || $unidade['itrid'] == 3){
            $text = 'PAR Análise do Estado '.$unidade['inudescricao'];
        }else if($unidade['itrid'] == 2){
            $text = "PAR Análise do Município ".$unidade['inudescricao']." - ".$unidade['estuf'];
        }
        return $text;

    }

    private function existeIniciativasFinalizadas(){
         $sql = "select COUNT(inp.inpid) as qt from par3.iniciativa_planejamento as inp
                    inner join workflow.documento as doc on doc.docid = inp.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    where
                    inp.inuid = {$this->instrumentoUnidade} and
                    esd.esdid not in ({$this->getNotInEstadoDocumentoIniciativaPlanejamento()})
                    ";
         try{
             $result = $this->pegaLinha($sql);
             return $result;
         } catch (Exception $e){
             throw new Exception($e->getMessage());
         }
    }
}


