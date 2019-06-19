<?php

/**
 * Metadados - cores, infromaÃ§Ãµes extras...
 */
Interface MetaDadosInterface{
	public function montaDado( $classeDado );
}
/**
 * @internal Abstract Factory
 */
class MetaDados implements MetaDadosInterface
{
    public $estuf = array();
    public $muncod = array();
    public $orgid;
    public $subid;
    public $pneano;
    public $tpdid;
    public $tpmapa;

    protected $metaDados = array();
    protected $Dado;

    /**
     * @param string $classeDado
     */
    public function montaDado( $classeDado ){

        $this->autoloadMetadados($classeDado);
        $this->Dado = new $classeDado();

        if( !empty($this->estuf) ) $this->Dado->estuf = $this->estuf;
        if( !empty($this->muncod) ) $this->Dado->muncod = $this->muncod;
        if( !empty($this->orgid) ) $this->Dado->orgid = $this->orgid;
        if( !empty($this->subid) ) $this->Dado->subid = $this->subid;
        if( !empty($this->pneciclo) ) $this->Dado->pneciclo = $this->pneciclo;
        if( !empty($this->tpdid) ) $this->Dado->tpdid = $this->tpdid;
        if( !empty($this->tpmapa) ) $this->Dado->tpmapa = $this->tpmapa;
        $this->Dado->carregaDado();
    }

    /**
     * Aplica Cores aos MunicÃ­pios
     *
     * @param PoligonosInterface $Poligonos
     * @return void
     * @internal exige que poligonos carregados tenham carregado municÃ­pios
     * @todo como validar que os poligonos representam estados ou municipios?
     */
    public function aplicaCoresAosMunicipios( PoligonosInterface $Poligonos ){

        $Poligonos->poligono = $this->aplicaCores( 'muncod', $Poligonos );

    }

    /**
     * Aplica Cores aos Estados
     *
     * @param PoligonosInterface $Poligonos
     * @return void
     * @internal exige que poligonos carregados tenham carregado estados
     */
    public function aplicaCoresAosEstados( PoligonosInterface $Poligonos ){
        $Poligonos->poligono = $this->aplicaCores( 'estuf', $Poligonos );

    }

    /**
     * Aplica Cores
     *
     * @param string $dimensao ('estuf',...)
     * @return void
     */
    public function aplicaCores( $dimensao, PoligonosInterface $Poligonos ){
        $poligonosDecodificados = json_decode( $Poligonos->poligono );

        if( !is_array($poligonosDecodificados) ) return false;

            if( !$poligonosDecodificados[0]->$dimensao ) return false;

            if( count($poligonosDecodificados) > 0 )
                foreach ($poligonosDecodificados as $chave => $valor) {

                    if( count($this->Dado->dado) > 0 && is_array($this->Dado->dado) )

                        foreach ($this->Dado->dado as $chave2 => $valor2) {

                            if( $valor->$dimensao == $valor2[$dimensao] && $valor->cor != $valor2['cor'] ){
                                $poligonosDecodificados[ $chave ]->cor = $valor2['cor'];
                                $poligonosDecodificados[ $chave ]->situacao = (string) ($valor2['situacao']);
                            }
                        }
                }
        return simec_json_encode( $poligonosDecodificados);
        //return json_encode( $poligonosDecodificados);
    }

    /**
     * Aplica dados complementares
     *
     * @param array $dados ('estuf',...)
     * @return void
     */
    public function aplicaDadosComplementares($dados, PoligonosInterface $Poligonos){
        $poligonosDecodificados = simec_json_decode( $Poligonos->poligono );
        $dado = $this->Dado->dado;
        if(!is_array($poligonosDecodificados)){ return false; }
        if(empty($dados)){ return false; }
        if( count($poligonosDecodificados) > 0 )
            foreach ($poligonosDecodificados as $chave => $valor) {
                if( count($this->Dado->dado) > 0 && is_array($this->Dado->dado) )
                    foreach ($dado as $chave2 => $valor2) {
                        if( $valor->muncod == $valor2['muncod'] ) {
                            foreach ($dados as $d) {
                                $poligonosDecodificados[$chave]->$d = $valor2[$d];
                            }
                        }
                    }
            }
        $Poligonos->poligono = simec_json_encode($poligonosDecodificados);
    }

    /**
     * Verifica se a classe de metadados está disponível e, caso contrário, tenta incluí-la no arquivo.
     *
     * As classes definidas em arquivos externos devem estar dentro do diretório /sase/classes/Mapa/Metadados e
     * podem ter extensão '.php' ou '.class.inc'.
     *
     * @param string $classe
     * @return boolean
     * @throws Exception Exceção lançada caso a classe não esteja definida neste arquivo, nem declarada em um arquivo externo seguindo as orientações acima.
     */
    protected function autoloadMetadados($classe)
    {
        // -- Classes declaradas dentro deste mesmo arquivo
        if (class_exists($classe)) {
            return;
        }

        // -- Classes declaradas dentro de /Metadados com extensão class.inc
        if (is_file(dirname(__FILE__) . "/Metadados/{$classe}.class.inc")) {
            require_once dirname(__FILE__) . "/Metadados/{$classe}.class.inc";
            return true;
        }

        // -- Classes declaradas dentro de /Metadados com extensão php
        if (is_file(dirname(__FILE__) . "/Metadados/{$classe}.php")) {
            require_once dirname(__FILE__) . "/Metadados/{$classe}.php";
            return true;
        }

        // -- Classe não encontrada em nenhum dos locais acima
        throw new Exception("A classe de metadados solicitada não está implementada ({$classe}).");
    }
}

/**
 * Interface dos Dados
 */
Interface DadosInterface{
    public function carregaDado();
}

interface DadosLegendaInterface
{
    public function dadosDaLegenda();
}

/**
 * Classe abstrata para funÃ§Ãµes que sejam
 * aplicÃ¡veis a todos os dados de forma semelhante
 */
class DadosAbstract
{
    public $estuf = array();
    public $orgid = array();
    public $subid = array();
    public $pneano;
    public $tpdid;
    public $tpmapa;

	public $dado;

    public function setEstuf($estuf)
    {
        $this->estuf = $estuf;
        return $this;
    }
}

/**
 * Dado - Situação Assessoramento Estado
 */
class DadoSituacaoAssessoramentoEstado extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento por Estado
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;

        $sql = "
                SELECT
                    s.stacor as cor,
                    CASE
                        WHEN s.stacod IS NULL THEN 'Não Cadastrado'
                        ELSE s.stadsc
                    END as situacao,
                    a.estuf as estuf
                FROM sase.assessoramentoestado a
                --INNER JOIN workflow.documento d ON d.docid = a.docid
                --INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid
                INNER JOIN sase.situacaoassessoramento s ON a.stacod = s.stacod
                WHERE 1=1
                ".( (is_array($this->estuf))?" AND a.estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - Situação Assessoramento Estado
 */
class DadoSituacaoAssessoramentoEstadoLei extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento por Estado
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;
        $sql = "
                SELECT
                        CASE
                            WHEN aseleipne IS NOT NULL THEN '#006600'
                            ELSE '#FFFFFF'
                        END as cor,
                        CASE
                            WHEN aseleipne IS NOT NULL THEN 'Com lei PNE'
                            ELSE 'Sem lei PNE'
                        END as situacao,
                        estuf as estuf
                FROM sase.assessoramentoestado
                WHERE 1=1
                ".( (is_array($this->estuf))?" AND estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - Situação Assessoramento Estado
 */
class DadoSituacaoAssessoramentoEstadoAdesao extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento por Estado
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
                SELECT
                        CASE
                            WHEN aseadesao IS NOT NULL THEN '#006600'
                            ELSE '#FFFFFF'
                        END as cor,
                        CASE
                            WHEN aseadesao IS NOT NULL THEN 'Com Adesão'
                            ELSE 'Sem Adesão'
                        END as situacao,
                        estuf as estuf
                FROM sase.assessoramentoestado
                WHERE 1=1
                ".( (is_array($this->estuf))?" AND estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - Situação Assessoramento Município
 */
class DadoSituacaoAssessoramentoMunicipioLeiPNE extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento por Muncípio Com Lei PNE
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
 	       SELECT
                    CASE
                            WHEN a.assleipne IS NOT NULL THEN '#006600'
                            ELSE '#FFFFFF'
                    END as cor,
                    CASE
                            WHEN a.assleipne IS NOT NULL THEN 'Com lei PNE'
                            ELSE 'Sem lei PNE'
                    END as situacao,
                    a.muncod as muncod,
                    a.estuf as estuf
                FROM sase.assessoramento a
                WHERE 1=1
                ".( (is_array($this->estuf))?" AND a.estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - Situação Assessoramento Município
 */
class DadoSituacaoAssessoramentoMunicipioAdesao extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento por Muncípio Com Adesão PNE
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
 	       SELECT
                    CASE
                            WHEN assadesao IS NOT NULL THEN '#006600'
                            ELSE '#FFFFFF'

                    END as cor,
                    CASE
                            WHEN assadesao IS NOT NULL THEN 'Com Adesão'
                            ELSE 'Sem Adesão'
                    END as situacao,
                    muncod as muncod,
                    estuf as estuf
                FROM sase.assessoramento
                WHERE 1=1
                ".( (is_array($this->estuf) ) ? " AND estuf IN ('".(implode("','",$this->estuf))."') " : "" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - SituaÃ§Ã£o QuestÃµes Pontuais
 */
class DadoSituacaoQuestoesPontuais extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da SituaÃ§Ã£o de QuestÃµes Pontuais
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;

        $sql = "
                SELECT
                        s.sqpcor as cor,
                        CASE
                                WHEN s.sqpcod IS NULL THEN 'NÃ£o Cadastrado'
                                ELSE s.sqpdsc
                        END as situacao,
                        a.muncod as muncod,
                        a.estuf as estuf
                FROM sase.questoespontuaispar a
                JOIN sase.situacaoquestoespontuais s ON s.sqpcod = a.sqpcod
                WHERE 1=1
                ".( (is_array($this->EstadosSolicitados))?" AND a.estuf IN ('".(implode("','",$this->EstadosSolicitados))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - MesoregiÃ£o
 */
class DadoMesoregiao extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da SituaÃ§Ã£o de MesoregiÃ£o
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;

        $sql = "
            SELECT
                    m.mescor as cor,
                    m.mesdsc as situacao,
                    t.muncod as muncod,
                    mun.estuf as estuf
            FROM territorios.municipio mun
            JOIN sase.territorio t ON mun.muncod = t.muncod
            JOIN sase.mesoregiao m ON m.mesid = t.mesid
            JOIN sase.orgaoestado oes ON m.oesid = oes.oesid
            WHERE terstatus = 'A'
            ".( (is_array( $this->estuf))?" AND mun.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
            " . ( ( $this->orgid )?" AND oes.orgid = '" . $this->orgid . "' ":"" ) . "
        ";

        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Plano de carreira dos professores
 */

class DadoPlanoCarreiraMunicipio extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da SituaÃ§Ã£o de MesoregiÃ£o
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
            select
                case
                    when pcp.docid is null and {$this->tpdid} <> 242 and {$this->tpdid} <> 244  and {$this->tpdid} <> 246 then '#FFFFFF'
                    when pcp.docid is null and {$this->tpdid} = 242 and pcppcr IS NULL then '#FF0000'
                    when pcp.docid is null and {$this->tpdid} = 242 and pcppcr IS NOT NULL then '#006600'
                    when pcp.docid is null and {$this->tpdid} = 244 and pcpadesao IS NULL then '#FF0000'
                    when pcp.docid is null and {$this->tpdid} = 244 and pcpadesao IS NOT NULL then '#006600'
                    else CASE
                        when {$this->tpdid} = 242 and pcppcr IS NULL then '#FF0000'
                        when {$this->tpdid} = 242 and pcppcr IS NOT NULL then '#006600'
                        when {$this->tpdid} = 244 and pcpadesao IS NULL then '#FF0000'
                        when {$this->tpdid} = 244 and pcpadesao IS NOT NULL then '#006600'
                        when {$this->tpdid} = 246 and pcpato IS NULL then '#FF0000'
                        when {$this->tpdid} = 246 and pcpato IS NOT NULL then '#006600'
                        when {$this->tpdid} = 240 then spm.spccor
                        when {$this->tpdid} = 238 then spc.spccor
                    end
                end as cor,
                case
                    when pcp.docid is null and {$this->tpdid} <> 242 and {$this->tpdid} <> 244 and {$this->tpdid} <> 246 then 'Sem Informação'
                    when pcp.docid is null and {$this->tpdid} = 242  and pcppcr IS NULL then 'Sem PCR'
                    when pcp.docid is null and {$this->tpdid} = 242  and pcppcr IS NOT NULL then 'Com PCR'
                    when pcp.docid is null and {$this->tpdid} = 244  and pcpadesao IS NULL then 'Sem PCR'
                    when pcp.docid is null and {$this->tpdid} = 244  and pcpadesao IS NOT NULL then 'Com PCR'
                    else CASE
                        when {$this->tpdid} = 242 then 'PCR'
                        when {$this->tpdid} = 244 then 'Termo de Adesão'
                        when {$this->tpdid} = 246 then 'Ato Legal da Comissão'
                        when {$this->tpdid} = 240 then spm.spmdsc
                        when {$this->tpdid} = 238 then spc.spcdsc
                    end
                end as situacao,
                pcp.muncod as muncod,
                pcp.estuf as estuf
            from sase.planocarreiraprofessor pcp
            left join workflow.documento doc on doc.docid = pcp.docid
            left join workflow.documento doc2 on doc2.docid = pcp.docid2
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
			WHERE 1=1
			".( (is_array( $this->estuf))?" AND pcp.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
		";
        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Planod de carreira por estado
 */
class DadoPlanoCarreiraEstado extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da SituaÃ§Ã£o de MesoregiÃ£o
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined'){ $this->estuf = '';};
        $sql = "
            select
                case
                    when pce.docid is null and {$this->tpdid} <> 242 and {$this->tpdid} <> 244 and {$this->tpdid} <> 246 then '#FFFFFF'
                    when pce.docid is null and {$this->tpdid} = 242 and pcepcr IS NULL then '#FF0000'
                    when pce.docid is null and {$this->tpdid} = 242 and pcepcr IS NOT NULL then '#006600'
                    when pce.docid is null and {$this->tpdid} = 244 and pceadesao IS NULL then '#FF0000'
                    when pce.docid is null and {$this->tpdid} = 244 and pceadesao IS NOT NULL then '#006600'
                    when pce.docid is null and {$this->tpdid} = 246 and pceato IS NULL then '#FF0000'
                    when pce.docid is null and {$this->tpdid} = 246 and pceato IS NOT NULL then '#006600'
                    else CASE
                            when {$this->tpdid} = 242 and pcepcr IS NULL then '#FF0000'
                            when {$this->tpdid} = 242 and pcepcr IS NOT NULL then '#006600'
                            when {$this->tpdid} = 244 and pceadesao IS NULL then '#FF0000'
                            when {$this->tpdid} = 244 and pceadesao IS NOT NULL then '#006600'
                            when {$this->tpdid} = 246 and pceato IS NULL then '#FF0000'
                            when {$this->tpdid} = 246 and pceato IS NOT NULL then '#006600'
                            when {$this->tpdid} = 240 then spm.spccor
                            when {$this->tpdid} = 238 then spc.spccor
                          end
                end as cor,
                case
                    when pce.docid is null and {$this->tpdid} <> 242 and {$this->tpdid} <> 244  and {$this->tpdid} <> 246 then 'Sem Informação'
                    when pce.docid is null and {$this->tpdid} = 242  and pcepcr IS NULL then 'Sem PCR'
                    when pce.docid is null and {$this->tpdid} = 242  and pcepcr IS NOT NULL then 'Com PCR'
                    when pce.docid is null and {$this->tpdid} = 244  and pceadesao IS NULL then 'Sem Termo de Adesão'
                    when pce.docid is null and {$this->tpdid} = 244  and pceadesao IS NOT NULL then 'Com Termo de Adesão'
                    when pce.docid is null and {$this->tpdid} = 246  and pceato IS NULL then 'Sem Ato Legal da Comissão'
                    when pce.docid is null and {$this->tpdid} = 246  and pceato IS NOT NULL then 'Com Ato Legal da Comissão'
                    else CASE
                            when {$this->tpdid} = 242 then 'PCR'
                            when {$this->tpdid} = 244 then 'Termo de Adesão'
                            when {$this->tpdid} = 246 then 'Ato Legal da Comissão'
                            when {$this->tpdid} = 240 then spm.spmdsc
                            when {$this->tpdid} = 238 then spc.spcdsc
                          end
                end as situacao,
                pce.estuf as estuf
            from sase.planocarreiraprofessorestado pce
            left join workflow.documento doc on doc.docid = pce.docid
            left join workflow.documento doc2 on doc2.docid = pce.docid2
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
			WHERE 1=1
			".( (is_array( $this->estuf))?" AND pce.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
		";
        print($sql);
        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Plano de carreira por estado - externo
 */
class DadoPlanocarreiraEstadoExterno extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Estado
     *
     * @return void
     * @internal carrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        $sql = "
            select
                case
                    when pce.docid is null and {$this->tpdid} <> 242 and {$this->tpdid} <> 244 and {$this->tpdid} <> 246 then '#FFFFFF'
                    when pce.docid is null and {$this->tpdid} = 242 and pcepcr IS NULL then '#FF0000'
                    when pce.docid is null and {$this->tpdid} = 242 and pcepcr IS NOT NULL then '#006600'
                    when pce.docid is null and {$this->tpdid} = 244 and pceadesao IS NULL then '#FF0000'
                    when pce.docid is null and {$this->tpdid} = 244 and pceadesao IS NOT NULL then '#006600'
                     when pce.docid is null and {$this->tpdid} = 246 and pceato IS NULL then '#FF0000'
                    when pce.docid is null and {$this->tpdid} = 246 and pceato IS NOT NULL then '#006600'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spccor
                            when {$this->tpdid} = 238 then spc.spccor
                            when {$this->tpdid} = 242 and pcepcr IS NULL then '#FF0000'
                            when {$this->tpdid} = 242 and pcepcr IS NOT NULL then '#006600'
                            when {$this->tpdid} = 244 and pceadesao IS NULL then '#FF0000'
                            when {$this->tpdid} = 244 and pceadesao IS NOT NULL then '#006600'
                            when {$this->tpdid} = 246 and pceato IS NULL then '#FF0000'
                            when {$this->tpdid} = 246 and pceato IS NOT NULL then '#006600'
                          end
                end as cor,
                case
                    when pce.docid is null and {$this->tpdid} <> 242 and {$this->tpdid} <> 244 and {$this->tpdid} <> 246 then 'Sem Informação'
                    when pce.docid is null and {$this->tpdid} = 242  and pcepcr IS NULL then 'Sem PCR'
                    when pce.docid is null and {$this->tpdid} = 242  and pcepcr IS NOT NULL then 'Com PCR'
                    when pce.docid is null and {$this->tpdid} = 244  and pceadesao IS NULL then 'Sem PCR'
                    when pce.docid is null and {$this->tpdid} = 244  and pceadesao IS NOT NULL then 'Com PCR'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spmdsc
                            when {$this->tpdid} = 238 then spc.spcdsc
                            WHEN {$this->tpdid} = 242 and pceadesao IS NOT NULL THEN 'Com Termo de Adesão'
                            WHEN {$this->tpdid} = 242 and pceadesao IS NULL THEN 'Sem Termo de Adesão'
                            WHEN {$this->tpdid} = 244 and pceadesao IS NOT NULL THEN 'Com PCR'
                            WHEN {$this->tpdid} = 244 and pceadesao IS NULL THEN 'Sem PCR'
                            WHEN {$this->tpdid} = 246 and pceato IS NOT NULL THEN 'Com Ato Legal da Comissão'
                            WHEN {$this->tpdid} = 246 and pceato IS NULL THEN 'Sem Ato Legal da Comissão'
                          end
                end as situacao,
                pce.estuf as estuf
            from sase.planocarreiraprofessorestado pce
            left join workflow.documento doc on doc.docid = pce.docid
            left join workflow.documento doc2 on doc2.docid = pce.docid2
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid";
        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Acompanhamento PNE - Município
 */
class DadoAcompanhoPneMunicipio extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da SituaÃ§Ã£o de MesoregiÃ£o
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        
        if ($this->estuf === 'undefined') $this->estuf = '';
        if ($this->muncod === 'undefined') $this->muncod = '';
        
        if (is_array( $this->estuf) && $this->estuf[0] == "") $this->estuf = '';
        if (is_array( $this->muncod) && $this->muncod[0] == "") $this->muncod = '';
        
        $coluna = 'pne.pnevalor';
        $pneciclo = $this->pneciclo;
        $subid = $this->subid;

        $colAnoVal = "(select paeano from sase.pneanoexibicaodial where paetipo = 'M' and subiddial = {$subid} order by paeid desc limit 1)";

        $sql = "with temp_cor as (select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A'),
                temp_legfinal as (select legfxa1, legfxa2, legcor from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A' order by legfxa2 desc limit 1)
                select
                    pne.muncod, 
                    pne.estuf, 
                    case
                        when {$coluna} is null or pne.pnevalor > (select legfxa2 from temp_legfinal) then '#FFFFFF'
                        --when pne.pnevalor > (select legfxa2 from temp_legfinal) then (select legcor from temp_legfinal)
                        else (select
                                legcor
                            from temp_cor
                            where {$coluna} between legfxa1 and legfxa2)
                    end as cor,
                    case
                        when {$coluna} is null or pne.pnevalor > (select legfxa2 from temp_legfinal) then 'Sem informaçao'
                        --when pne.pnevalor > (select legfxa2 from temp_legfinal) then (select 'Faixa de '||legfxa1||' à '||legfxa2 from temp_legfinal)
                        else (select
                                'Faixa de '||legfxa1||' à '||legfxa2
                            from temp_cor
                            where {$coluna} between legfxa1 and legfxa2)
                    end as situacao
                from sase.pnedial pne
                inner join territorios.municipio mun on pne.muncod = mun.muncod
                left join sase.pneprevdial ppd on ppd.pneiddial = pne.pneiddial
                inner join sase.pneanoexibicaodial anoexib on pne.pneano = anoexib.paeano and pne.subiddial = anoexib.subiddial
                where anoexib.paetipo = 'M'
                AND anoexib.ciclo = {$pneciclo}
                and pne.pneciclo = {$pneciclo}
                and pne.subiddial = {$subid}
                and anoexib.paestatus = 'A'
                ".( (is_array( $this->muncod) )?" and pne.muncod  IN ('".(implode("','", $this->muncod))."') ":"" )."
                order by mun.estuf, mun.mundescricao, ppd.ppdid ";

        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Plano de carreira - Município
 */
class DadoMacroPlanoCarreiraExterno extends  DadosAbstract implements DadosInterface
{
    public function setEstuf($estuf) {
        parent::setEstuf($estuf);
    }
    public function carregaDado(){
        global $db;
//                $sql = "
//                    select
//                        case
//                            when pcp.docid is null and {$_SESSION['tipo']} <> 242 and {$_SESSION['tipo']} <> 244 then '#FFFFFF'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 242 and pcppcr IS NULL then '#FF0000'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 242 and pcppcr IS NOT NULL then '#006600'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 244 and pcpadesao IS NULL then '#FF0000'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 244 and pcpadesao IS NOT NULL then '#006600'
//                            else CASE
//                                when {$_SESSION['tipo']} = 242 and pcppcr IS NULL then '#FF0000'
//                                when {$_SESSION['tipo']} = 242 and pcppcr IS NOT NULL then '#006600'
//                                when {$_SESSION['tipo']} = 244 and pcpadesao IS NULL then '#FF0000'
//                                when {$_SESSION['tipo']} = 244 and pcpadesao IS NOT NULL then '#006600'
//                                when {$_SESSION['tipo']} = 240 then spm.spccor
//                                when {$_SESSION['tipo']} = 238 then spc.spccor
//                            end
//                        end as cor,
//                        case
//                            when pcp.docid is null and {$_SESSION['tipo']} <> 242 and {$_SESSION['tipo']} <> 244 then 'Sem Informação'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 242  and pcppcr IS NULL then 'Sem PCR'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 242  and pcppcr IS NOT NULL then 'Com PCR'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 244  and pcpadesao IS NULL then 'Sem PCR'
//                            when pcp.docid is null and {$_SESSION['tipo']} = 244  and pcpadesao IS NOT NULL then 'Com PCR'
//                            else CASE
//                                when {$_SESSION['tipo']} = 242 then '#FFFFFF'
//                                when {$_SESSION['tipo']} = 244 then '#FFFFFF'
//                                when {$_SESSION['tipo']} = 240 then spm.spmdsc
//                                when {$_SESSION['tipo']} = 238 then spc.spcdsc
//                            end
//                        end as situacao,
//                        pcp.muncod as muncod,
//                        pcp.estuf as estuf
//                    from sase.planocarreiraprofessor pcp
//                    left join workflow.documento doc on doc.docid = pcp.docid
//                    left join workflow.documento doc2 on doc2.docid = pcp.docid2
//                    left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
//                    left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
//                    WHERE 1=1
//                    ".( (is_array( $this->estuf))?" AND pcp.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
//                ";
//        ver($sql);die;
                                
    //global $db;
        //if ($this->estuf === 'undefined') $this->estuf = '';
        
        $sql = "
            select
                case
                    when pcp.docid is null and {$_SESSION['tipo']} <> 242 and {$_SESSION['tipo']} <> 244 then '#FFFFFF'
                    when pcp.docid is null and {$_SESSION['tipo']} = 242 and pcppcr IS NULL then '#FF0000'
                    when pcp.docid is null and {$_SESSION['tipo']} = 242 and pcppcr IS NOT NULL then '#006600'
                    when pcp.docid is null and {$_SESSION['tipo']} = 244 and pcpadesao IS NULL then '#FF0000'
                    when pcp.docid is null and {$_SESSION['tipo']} = 244 and pcpadesao IS NOT NULL then '#006600'
                    else CASE
                        when {$_SESSION['tipo']} = 242 and pcppcr IS NULL then '#FF0000'
                        when {$_SESSION['tipo']} = 242 and pcppcr IS NOT NULL then '#006600'
                        when {$_SESSION['tipo']} = 244 and pcpadesao IS NULL then '#FF0000'
                        when {$_SESSION['tipo']} = 244 and pcpadesao IS NOT NULL then '#006600'
                        when {$_SESSION['tipo']} = 240 then spm.spccor
                        when {$_SESSION['tipo']} = 238 then spc.spccor
                    end
                end as cor,
                case
                    when pcp.docid is null and {$_SESSION['tipo']} <> 242 and {$_SESSION['tipo']} <> 244 then 'Sem Informação'
                    when pcp.docid is null and {$_SESSION['tipo']} = 242  and pcppcr IS NULL then 'Sem PCR'
                    when pcp.docid is null and {$_SESSION['tipo']} = 242  and pcppcr IS NOT NULL then 'Com PCR'
                    when pcp.docid is null and {$_SESSION['tipo']} = 244  and pcpadesao IS NULL then 'Sem PCR'
                    when pcp.docid is null and {$_SESSION['tipo']} = 244  and pcpadesao IS NOT NULL then 'Com PCR'
                    else CASE
                        when {$_SESSION['tipo']} = 242 then '#FFFFFF'
                        when {$_SESSION['tipo']} = 244 then '#FFFFFF'
                        when {$_SESSION['tipo']} = 240 then spm.spmdsc
                        when {$_SESSION['tipo']} = 238 then spc.spcdsc
                    end
                end as situacao,
                pcp.muncod as muncod,
                pcp.estuf as estuf
            from sase.planocarreiraprofessor pcp
            left join workflow.documento doc on doc.docid = pcp.docid
            left join workflow.documento doc2 on doc2.docid = pcp.docid2
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
			WHERE 1=1 AND pcp.estuf IN ('MG')";
                        
        $this->dado = $db->carregar( $sql );
                                
       // $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - Monitoramento PNE - Estado
 */
class DadoMonitoramentoPneEstadoExterno extends DadosAbstract implements DadosInterface
{
    public function carregaDado(){
        global $db;
        $subid = $this->subid;
        $sql = <<<DML
            with temp_cor as (select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A')
            select
                estuf,
                case
                    when pnevalor > (select legfxa2 from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A' order by legfxa2 desc limit 1) then (select legcor from sase.legendaindicadores where subiddial = {$subid} order by legfxa2 desc limit 1)
                    else cor
                end as cor,
                case
                    when pnevalor > (select legfxa2 from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A' order by legfxa2 desc limit 1) then (select 'Faixa de '||legfxa1||' à '||legfxa2 from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A' order by legfxa2 desc limit 1)
                    else situacao
                end as situacao
            from (select
                pne.estuf,
                pne.pnevalor,
                case
                    when pne.pnevalor is null then '#FFFFFF'
                    else (
                    select
                        legcor
                    from temp_cor
                    where pne.pnevalor between legfxa1 and legfxa2
                    )
                end as cor,
                case
                    when pne.pnevalor is null then 'Sem informaçao'
                    else (select
                    'Faixa de '||legfxa1||' à '||legfxa2
                    from temp_cor
                    where pne.pnevalor between legfxa1 and legfxa2)
                end as situacao
                from sase.pnedial pne
                where pne.pnetipo = 'E'
                and pneano = (select paeano from sase.pneanoexibicaodial where subiddial = {$subid} and paetipo = 'E' order by paeid desc limit 1)
                and pne.estuf is not null
                and pne.subiddial = {$subid}) tab
DML;
//        $sql = <<<DML
//            with temp_cor as (select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subiddial = {$subid})
//            select
//                pne.estuf,
//                case
//                    when pne.pnevalor is null then '#FFFFFF'
//                    else (
//                        select
//                            legcor
//                        from temp_cor
//                        where pne.pnevalor between legfxa1 and legfxa2
//                    )
//                end as cor,
//                case
//                    when pne.pnevalor is null then 'Sem informaçao'
//                    else (select
//                        'Faixa de '||legfxa1||' à '||legfxa2
//                        from temp_cor
//                        where pne.pnevalor between legfxa1 and legfxa2)
//                end as situacao
//            from sase.pnedial pne
//            where pne.pnetipo = 'E'
//            and pneano = (select paeano from sase.pneanoexibicaodial where subiddial = {$subid} and paetipo = 'E')
//            and pne.estuf is not null
//            and pne.subiddial = {$subid}
//DML;
        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Monitoramento PNE - Município
 */
class DadoMonitoramentoPneMunicipioExterno extends DadosAbstract implements DadosInterface
{
    public function carregaDado(){
        global $db;
        $subid = $this->subid;
        $estuf = implode("'),upper('", $this->estuf);
        $sql = <<<DML
            with temp_cor as (select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A')
            select
                pne.muncod,
                mun.mundescricao,
                round(pne.pnevalor, 2) as pnevalor,
                pne.pneano,
                case
                    when pne.pnevalor is null then '#FFFFFF'
                    else (
                        select
                            legcor
                        from temp_cor
                        where pne.pnevalor between legfxa1 and legfxa2
                    )
                end as cor,
                case
                    when pne.pnevalor is null then 'Sem informaçao'
                    else (select
                        'Faixa de '||legfxa1||' à '||legfxa2
                        from temp_cor
                        where pne.pnevalor between legfxa1 and legfxa2)
                end as situacao
            from sase.pnedial pne
            inner join territorios.municipio mun on pne.muncod = mun.muncod
            where pne.pnetipo = 'M'
            and pneano = (select paeano from sase.pneanoexibicaodial where subiddial = {$subid} and paetipo = 'M')
            and mun.estuf in (upper('{$estuf}'))
            and pne.subiddial = {$subid}
DML;
        $this->dado = $db->carregar( $sql );
    }
}


/**
 * Dado - Acompanhamento PNE - Estado
 */
class DadoAcompanhoPneEstado extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da SituaÃ§Ã£o de MesoregiÃ£o
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        if (is_array( $this->estuf) && $this->estuf[0] == "") $this->estuf = '';
        
        
        $coluna = 'pne.pnevalor';
        $subid = $this->subid;
        $pnevalor = $db->pegaUm("SELECT pnevalor from sase.pnedial pne
                                INNER JOIN territorios.estado est on pne.estuf = est.estuf
                                LEFT JOIN sase.pneprevdial ppd on ppd.pneiddial = pne.pneiddial
                                WHERE pnetipo = 'E'
                                AND pneciclo = {$this->pneciclo}
                                AND subiddial = {$subid}
                                ".( (is_array( $this->estuf) )?"
                                AND est.estuf IN ('".(implode("','", $this->estuf))."') ":"" )." ");
        $pneciclo = empty($pnevalor) ? $this->pneciclo -1 : $this->pneciclo;
        $colAno = "and pneano = (select DISTINCT paeano from sase.pneanoexibicaodial where subiddial = {$subid} and paetipo = 'E')";

        $sql = "with 
                temp_cor as (select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subiddial = {$subid} and legstatus = 'A'),
                temp_legfinal as (select legfxa1, legfxa2, legcor from sase.legendaindicadores where subiddial = 16 and legstatus = 'A' order by legfxa2 desc limit 1)
                select distinct
                    est.estuf,
                    case
                        when {$coluna} is null or pne.pnevalor > (select legfxa2 from temp_legfinal) then '#FFFFFF'
                        else (select legcor from temp_cor where {$coluna} between legfxa1 and legfxa2)
                    end as cor,
                    case
                        when {$coluna} is null or pne.pnevalor > (select legfxa2 from temp_legfinal) then 'Sem informaçao'
                        else (select 'Faixa de '||legfxa1||' à '||legfxa2 from temp_cor where {$coluna} between legfxa1 and legfxa2)
                    end as situacao
                from sase.pnedial pne
                inner join territorios.estado est on pne.estuf = est.estuf
                left join sase.pneprevdial ppd on ppd.pneiddial = pne.pneiddial
                inner join sase.pneanoexibicaodial anoexib on pne.pneano = anoexib.paeano and pne.subiddial = anoexib.subiddial
                where anoexib.paetipo = 'E'
                and anoexib.ciclo = {$pneciclo}
                and pne.pneciclo = {$pneciclo}
                and pne.subiddial = {$subid}
                ".( (is_array( $this->estuf) )?" and est.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
                order by est.estuf";

        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Preenchimento Ficha Monitoramento Município (Ficha Técnica do Plano de Educação Município) 
 */
class DadoFichaMonitoramento extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Ficha de monitoramento por Muncípio Preenchido
     *
     * @return void
     * @internal carrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
 	       SELECT
                    CASE
                            WHEN usucpf IS NOT NULL AND fmtdatainclusao IS NOT NULL THEN '#006600'
                            ELSE '#FFFFFF'
                    END as cor,
                    CASE
                            WHEN usucpf IS NOT NULL AND fmtdatainclusao IS NOT NULL THEN 'Preenchido'
                            ELSE 'Não preenchido'
                    END as situacao,
                    fm.muncod as muncod,
                    fm.estuf as estuf
                FROM sase.fichamonitoramento fm
                WHERE 1=1
                ".( (is_array($this->estuf))?" AND estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Preenchimento Ficha Monitoramento Município (Ficha Técnica do Plano de Educação Município)
 */
class DadoAgendaTrabalhoComissaoCoordenadora extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Ficha de monitoramento por Muncípio Preenchido
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $where = ( (is_array($this->estuf))?" AND estuf IN ('".(implode("','",$this->estuf))."') ":"" );

        $sql = <<<DML
                SELECT  DISTINCT
                        CASE WHEN accdatainclusao IS NOT NULL
                                AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' AND accresponsavel != '' AND accprazo IS NOT NULL) ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 2 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 3 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 4 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )                         
                            THEN '#00cc00'
                            ELSE '#cc0000'
                        END AS cor,
                        CASE WHEN accdatainclusao IS NOT NULL
                                AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' AND accresponsavel != '' AND accprazo IS NOT NULL) ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 2 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 3 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                                --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 4 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )                         
                            THEN 'Preenchido'
                            ELSE 'Não preenchido'
                        END AS situacao,
                        m.muncod,
                        m.estuf
                FROM territorios.municipio AS m
                LEFT JOIN sase.agendacomissaocoordenadora AS a ON a.muncod = m.muncod
                WHERE 1=1
                {$where}
DML;

        $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - Preenchimento Ficha Monitoramento Estado (Ficha Técnica do Plano de Educação Estado) 
 */
class DadosFichaMonitoramentoEstado extends DadosAbstract implements DadosInterface
{
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
                SELECT
                    CASE
                        WHEN usucpf IS NOT NULL AND fmedatainclusao IS NOT NULL THEN '#006600'
                        ELSE '#FFFFFF'
                    END as cor,
                    CASE
                        WHEN usucpf IS NOT NULL AND fmedatainclusao IS NOT NULL THEN 'Preenchido'
                        ELSE 'Não preenchido'
                    END as situacao,
                    estuf as estuf
                FROM sase.fichamonitoramentoestado
                WHERE 1=1
                ".( (is_array($this->estuf))?" AND estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Preenchimento Agenda Monitoramento Estado (Ficha Técnica do Plano de Educação Estado)
 */
class DadoAgendaTrabalhoComissaoCoordenadoraEstado extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Ficha de monitoramento por Estado Preenchido
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $where = ( (is_array($this->estuf))?" AND e.estuf IN ('".(implode("','",$this->estuf))."') ":"" );

        $sql = <<<DML
                SELECT  DISTINCT
                    CASE WHEN acedatainclusao IS NOT NULL
                            AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado acc where eacid  = 1 and acestatus = 'A' AND acc.estuf = e.estuf AND ( aceacao != '' AND aceresponsavel != '' AND aceprazo IS NOT NULL) ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 1 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 2 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 3 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 4 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                        THEN '#00cc00'
                        ELSE '#cc0000'
                    END AS cor,
                    CASE WHEN acedatainclusao IS NOT NULL
                            AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado acc where eacid  = 1 and acestatus = 'A' AND acc.estuf = e.estuf AND ( aceacao != '' AND aceresponsavel != '' AND aceprazo IS NOT NULL) ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 1 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 2 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 3 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordestado where eacid  = 4 and acestatus = 'A' AND ( aceacao != '' OR aceresponsavel != '' OR aceprazo IS NOT NULL OR aceobservacao != '') ) )
                        THEN 'Preenchido'
                        ELSE 'Não preenchido'
                        END AS situacao,
                    e.estuf
                FROM territorios.estado AS e
                LEFT JOIN sase.agendacomissaocoordestado AS a ON a.estuf = e.estuf
                WHERE 1=1
                {$where}
DML;

        $this->dado = $db->carregar( $sql );
    }
}