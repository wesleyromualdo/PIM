<?php
/**
 * Classe de mapeamento da entidade par3.programa_fpm_municipio.
 *
 * @version $Id$
 * @since 2018.07.11
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Programa_fpm_municipio: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Programa_fpm_municipio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Programa_fpm_municipio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $fmpid Chave Primária da tabela - default: nextval('par3.programa_fpm_municipio_fmpid_seq'::regclass)
 * @property bigint $fpmvaloraplicadopnmemunicipio Total de Matrículas de 3º ao 9ª ano da Rede:
 * @property number $fpmqtmatriculaaptasatendimentocriterio1 Total de Matrículas atendidas pelo PDDE Integral:
 * @property bigint $docid id do estado documento no workflow
 * @property bigint $inuid id do instrumento unidade
 */
class Par3_Model_ProgramaFPMMunicipio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.programa_fpm_municipio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'fmpid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'fmpid' => null,
        'fpmvaloraplicadopnmemunicipio' => null,
        'fpmqtmatriculaaptasatendimentocriterio1' => null,
        'docid' => null,
        'inuid' => null,
    );

    private $etapas_ensino = "(4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,41) ";


    /**
     * @param $inuid
     * @return array|bool|mixed|NULL
     * @throws Exception
     */
    public function getDadosMunicipioByUnidade($inuid){
       $sql = "SELECT * FROM par3.programa_fpm_municipio WHERE inuid = {$inuid}";
       try{
        return $this->pegaLinha($sql);
       }catch (Exception $e){
           throw new Exception($e->getMessage());
       }
    }

    public function getDadosDaUnidade($inuid){
        $sql = "select * from territorios.municipio as mun 
                inner join par.instrumentounidade as inu on inu.muncod = mun.muncod
                where inu.inuid = {$inuid}";

        try{
            return $this->pegaLinha($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }



    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'fmpid' => array('Digits'),
            'fpmvaloraplicadopnmemunicipio' => array('allowEmpty' => true),
            'fpmqtmatriculaaptasatendimentocriterio1' => array('allowEmpty' => true),
            'docid' => array('allowEmpty' => true),
            'inuid' => array('allowEmpty' => true),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function salvaDadosMunicipio($dados){
        $fpmvaloraplicadopnmemunicipio = str_replace(".","",$dados['fpmvlaplicadopnmemunicipio']);
        $dados['fpmvaloraplicadopnmemunicipio'] = str_replace(",",".",$fpmvaloraplicadopnmemunicipio);
        //ver($dados['fpmvaloraplicadopnmemunicipio']);
        try{
            $this->popularDadosObjeto($dados);
            $this->salvar();
            return $this->commit();
        }catch (Exeption $e){
            throw new Exception($e->getMessage());
        }
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $prgid
     * @param $inuid
     * @return bool
     * @throws Exception
     * @description Este método verifica se o município já iniciou o fluxo
     */
    public function isFluxoIniciado($prgid, $inuid)
    {
        $sql = "select * from par3.prodesaoprograma 
                        where inuid = {$inuid} and 
                        pfaid = (select pfaid from par3.proadesao where prgid = {$prgid});";
        try {
            $result = $this->pegaLinha($sql);
            if (!empty($result['adpresposta'])) {
                return true;
            }
            return false;
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }
    }

    public function iniciaFluxo($docid, $inuid)
    {
        $arrayDados = array(
            'inuid' => $inuid,
            'docid' => $docid,
        );
        try {
            $this->popularDadosObjeto($arrayDados);
            $this->salvar();
            $result = $this->commit();
            return $result;
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }
    }

    public function fetchProgramaFPMByIdUnidade($inuid)
    {
        $sql = "select * from par3.programa_fpm_municipio where inuid = {$inuid}";
        try {
            return $this->pegaLinha($sql);
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }
    }

    /**
     * @param $inuid
     * @return array|bool|mixed|NULL
     * @throws Exception
     */
    public function carregaValueCamposMatriculas($inuid)
    {

        $sql = "select 
                    SUM(fund.qt_matricula_fundamental) as total_matricula_ensino_fundamental,
                    SUM(fund.vl_maiseducacao) as total_matriculas_atendidas_pdde_integral,
                    SUM(fund.vl_integral) as total_matricula_tempo_integral_censo,
                    fpmm.fpmvaloraplicadopnmemunicipio
                    from par3.programa_fpm_fundamental as fund
                    inner join par3.instrumentounidade as inu on inu.muncod::integer = fund.muncod
                    left join par3.programa_fpm_fundamental_repasse_fnde2018 as fnde on fnde.muncod::numeric = fund.muncod
                    left join par3.programa_fpm_municipio_escola as fpmesc on fpmesc.codinep = fund.codinep
                    left join par3.programa_fpm_municipio as fpmm on fpmm.inuid = inu.inuid
                    where 
                    inu.inuid = {$inuid}
                    group by fpmm.fpmvaloraplicadopnmemunicipio ";
        try {
            return $this->pegaLinha($sql);
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }

    }

    public function carregaDadosEscolasDaUnidade($inuid)
    {
        $sql = "select 
                    esc.co_entidade as codinep,
                    esc.no_entidade as escnome,
                    fund.mundescricao,
                    fund.uf as estuf,
                    case
                        when esc.tp_localizacao = 1 then 'Urbano'
                        when esc.tp_localizacao = 2 then 'Rural'
                    end as tp_localizacao,
                    fund.qt_matricula_fundamental as total_matricula_censo,
                    fund.vl_maiseducacao as total_matricula_pdde_integral,
                    fund.qt_matricula_fundamental_integral as total_matricula_censo_integral,
                    fpmesc.fpmevlestudantespnme15h
                    from par3.programa_fpm_fundamental as fund
                    inner join par3.instrumentounidade as inu on inu.muncod::numeric = fund.muncod
                    inner join educacenso_2017.ts_censo_basico_escola as esc on esc.co_entidade = fund.codinep
                    left join par3.programa_fpm_municipio_escola as fpmesc on fpmesc.codinep = fund.codinep
                    where inu.inuid = {$inuid} order by esc.no_entidade asc";

        try {
            return $this->carregar($sql, 60000);
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }
    }



    public function carregaDadosEscolasDaUnidadeJaComValores($inuid)
    {
        $sql = "select 
                    count(*) as qt
                    from par3.programa_fpm_fundamental as fund
                    inner join par3.instrumentounidade as inu on inu.muncod::numeric = fund.muncod
                    inner join educacenso_2017.ts_censo_basico_escola as esc on esc.co_entidade = fund.codinep
                    inner join par3.programa_fpm_municipio_escola as fpmesc on fpmesc.codinep = fund.codinep
                    where inu.inuid = {$inuid}";
        try {
            return $this->pegaLinha($sql);
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }
    }

    public function getDadosFnde2018ByUnidade($inuid)
    {
        $sql = "select vl_repassefnde2018 from par3.programa_fpm_fundamental_repasse_fnde2018 as fnde
                    inner join par3.instrumentounidade as inu on inu.muncod = fnde.muncod
                    where inu.inuid = {$inuid}
                    order by uf, fnde.mundescricao asc";
        try {
            return $this->pegaLinha($sql);
        } catch (Exception $e) {
            throw new Exception($e->getMessage());

        }

    }

    /**
     * @param $inuid
     * @param $docid
     * @description atualiza o documento do municipio
     */
    public function updateEstadoDocumento($inuid,$docid){
        $sql = "UPDATE par3.programa_fpm_municipio SET docid = {$docid} WHERE inuid =  {$inuid}";
        try {
            $this->executar($sql);
            return $this->commit();
        } catch (Exception $e) {
            throw new Exception($e->getMessage());

        }
    }

    public function getEsddesc($inuid){
        $sql = "select esd.* from
                par3.prodesaoprograma as prod
                inner join workflow.documento as doc on doc.docid = prod.docid
                inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                where inuid = {$inuid} and pfaid = 11;";
        try {
            return $this->pegaLinha($sql);
        } catch (Exception $e) {
            throw new Exception($e->getMessage());

        }
    }

    public function getRelatorioGeralExport(){
        $sql = "select 
                    SUM(fund.qt_matricula_fundamental) as total_matricula_ensino_fundamental,
                    SUM(fund.vl_maiseducacao) as total_matriculas_atendidas_pdde_integral,
                    SUM(fund.vl_integral) as total_matricula_tempo_integral_censo,
                    fpmm.fpmvaloraplicadopnmemunicipio
                    from par3.programa_fpm_fundamental as fund
                    inner join par3.instrumentounidade as inu on inu.muncod::integer = fund.muncod
                    left join par3.programa_fpm_fundamental_repasse_fnde2018 as fnde on fnde.muncod::numeric = fund.muncod
                    left join par3.programa_fpm_municipio_escola as fpmesc on fpmesc.codinep = fund.codinep
                    left join par3.programa_fpm_municipio as fpmm on fpmm.inuid = inu.inuid
                    order by";
    }

}
