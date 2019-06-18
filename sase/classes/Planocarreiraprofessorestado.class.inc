<?php


include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

/**
 * Sase_Model_Planocarreiraprofessorestado
 *
 * @category Class
 * @package  A1
 * @author   VICTOR MARTINS MACHADO <VictorMachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0
 * @link     no link
 */
class Sase_Model_Planocarreiraprofessorestado extends Modelo
{
    /**
     * @var FilesSimec Referência para a classe de gestão de arquivos.
     */
    protected $arquivo;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.planocarreiraprofessorestado';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pceid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pceid' => null,
        'estuf' => null,
        'muncod' => null,
        'usucpf' => null,
        'docid' => null,
        'pcestatus' => null,
        'docid2' => null,
        'pcepcr' => null,
        'pceadesao' => null,
        'pceato' => null,
        'pcempl' => null
    );

    /**
     * Retorna uma referencia para FilesSimec.
     *
     * @return FilesSimec
     */
    protected function getArquivo()
    {
        if (is_null($this->arquivo)) {
            $this->arquivo = new FilesSimec();
        }

        return $this->arquivo;
    }

    /**
     * Retorna informações de um arquivo.
     *
     * @param string $campoArquivo Nome do atributo que guarda o ID do arquivo.
     * @return array
     */
    public function getInfoArquivo($campoArquivo)
    {
        if (empty($this->$campoArquivo)) {
            return [];
        }
        return $this
            ->getArquivo()
            ->getDadosArquivo($this->$campoArquivo);
    }
}
