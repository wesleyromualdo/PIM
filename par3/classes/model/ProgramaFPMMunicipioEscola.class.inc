<?php
/**
 * Classe de mapeamento da entidade par3.programa_fpm_municipio_escola.
 *
 * @version $Id$
 * @since 2018.07.16
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Programa_fpm_municipio_escola: Tabela que guarda o valor que a escola deseja ter do programa
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Programa_fpm_municipio_escola($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Programa_fpm_municipio_escola($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $fpmeid Chave primária da tabela - default: nextval('par3.programa_fpm_municipio_escola_fpmeid_seq'::regclass)
 * @property bigint $codinep Código INEP da escola
 * @property bigint $fpmevlestudantespnme15h Campo que guardará o valor de Estudantes para atendimento do PNME 15h
 */
class Par3_Model_ProgramaFPMMunicipioEscola extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.programa_fpm_municipio_escola';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'fpmeid',
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
        'fpmeid' => null,
        'codinep' => null,
        'fpmevlestudantespnme15h' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'fpmeid' => array('Digits'),
            'codinep' => array('allowEmpty' => true),
            'fpmevlestudantespnme15h' => array('allowEmpty' => true),
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


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $codigoINEP
     * @return bool
     * @throws Exception
     * @description Este método verica se já existe um valor cadastrado para a escola (true), se não (false).
     *
     */
    public function valorJaCadastradoEscola($codigoINEP){
        if(!empty($codigoINEP)){
            $sql = "SELECT fpmevlestudantespnme15h from par3.programa_fpm_municipio_escola where codinep = {$codigoINEP};";
            try{
                $dados = $this->carregar($sql);
                return (!empty($dados)) ? true : false;
            }catch (Exception $e){
                throw new Exception($e->getMessage());
            }
        }else{
            throw new Exception("Código INEP é necessário!");
        }
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $array
     * @return bool
     * @description cria o sql que altera o valor da quantidade de estudantes que será atendida
     */
    public function updateValueSQL($array){
        $sql  = " UPDATE par3.programa_fpm_municipio_escola SET fpmevlestudantespnme15h = {$array[1]} WHERE codinep = {$array[0]}; \n";
        return $sql;
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $array
     * @return bool
     * @throws Exception
     * @description cria o sql que insere a quantidade de estudantes que será atendido da escola
     *
     */
    public function insertValueSQL($array){
        $array[1] = (is_null($array[1]) || empty($array[1])) ? 0 : $array[1];
        $sql = "INSERT INTO par3.programa_fpm_municipio_escola VALUES(nextval('par3.programa_fpm_municipio_escola_fpmeid_seq'),{$array[0]},{$array[1]}); \n";
        return $sql;
    }

    /**
     * @param $arrayDados
     * @throws Exception
     */
    public function salvaDadosEscolas($arrayDados){
        $sql = "";

        if(!empty($arrayDados) && count($arrayDados) > 0){
            foreach ($arrayDados['vlestudanteporescola'] as $d){
                $arrayValues = explode("|",$d);
                if($arrayValues[0]){
                    $dadosEscola = $this->getByCodInep($arrayValues[0]);
                    if(empty($dadosEscola['codinep'])){
                        $sql .= $this->insertValueSQL($arrayValues);
                    }else{
                        $sql .= $this->updateValueSQL($arrayValues);
                    }

                }
            }
            if(!empty($sql)){
                try{
                    $this->executar($sql);
                    return $this->commit();
                }catch (Exception $e){
                    throw new Exception($e->getMessage());
                }
            }
        }
    }

    /**
     * @param $codinep
     * @return bool
     * @throws Exception
     */
    public function getByCodInep($codinep){
        $sql = "SELECT * FROM par3.programa_fpm_municipio_escola where codinep = {$codinep}";
        try{
            return $this->pegaLinha($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }

    }


    /**
     * @param $inuid
     * @return bool|mixed|NULL|string
     * @throws Exception
     * @description Este método pega a somatória total da escola
     */
    public function getSomatoriaValoresEscolasPorEntidade($inuid){
        $sql = "select coalesce(sum(pfmme.fpmevlestudantespnme15h),0) as fpmevlestudantespnme15h from par3.programa_fpm_municipio_escola pfmme
                inner join par3.programa_fpm_fundamental as pfmf on pfmf.codinep = pfmme.codinep
                inner join par3.instrumentounidade as inu on inu.muncod::numeric = pfmf.muncod
                where inu.inuid = {$inuid}";
        try{
            return $this->pegaUm($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }


    public function fetchAllEscolasByIntrumentoUnidade($inuid){
        $sql = "select * from par3.programa_fpm_municipio_escola pfmme
                inner join par3.programa_fpm_fundamental as pfmf on pfmf.codinep = pfmme.codinep
                inner join par3.instrumentounidade as inu on inu.muncod::numeric = pfmf.muncod
                where inu.inuid = {$inuid}";
        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }


}
